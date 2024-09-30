from fastapi import FastAPI
from pydantic import BaseModel
import tensorflow as tf
from tensorflow.nn import softmax
from transformers import BertTokenizer, TFBertForSequenceClassification
import spacy
import json
import random  # Utilisé pour sélectionner aléatoirement une réponse parmi les possibles

class Question(BaseModel):
    question: str

app = FastAPI()

# Initialisation du tokenizer et du modèle
tokenizer = BertTokenizer.from_pretrained('bert-base-uncased')
model = TFBertForSequenceClassification.from_pretrained('bert-base-uncased', num_labels=15)
model.load_weights('../models/model_weights')

# Chargement de Spacy pour l'analyse des entités
nlps = spacy.load("../models/output/model-best")

# Chargement du mapping des labels et des réponses prédéfinies
with open('../data/label_mapping.json') as f:
    label_mapping = json.load(f)

with open('../data/class_responses.json') as f:
    class_responses = json.load(f)

# Chemin vers le fichier JSON de commande
order_file_path = '../data/order.json'

def prepare_predict_data(question: str, tokenizer):
    """Prépare les données pour la prédiction."""
    inputs = tokenizer.encode_plus(
        question, 
        return_tensors="tf", 
        add_special_tokens=True, 
        max_length=512, 
        padding="max_length", 
        truncation=True
    )
    return {key: tf.convert_to_tensor(value) for key, value in inputs.items()}

def initialize_order_file():
    """Réinitialise le fichier order.json à son état initial."""
    with open(order_file_path, 'w') as file:
        json.dump({"items": []}, file)

# Appel de la fonction d'initialisation au démarrage de l'application
initialize_order_file()

def load_order():
    """Charge ou initialise l'état de la commande."""
    try:
        with open(order_file_path, 'r') as file:
            return json.load(file)
    except (json.JSONDecodeError, FileNotFoundError):
        return {"items": []}

@app.post("/predict/")
async def predict(question: Question):
    try:
        order = load_order()
        inputs = prepare_predict_data(question.question, tokenizer)
        predictions = model.predict(inputs)
        probabilities = softmax(predictions.logits, axis=1).numpy()
        predicted_class_index = tf.argmax(probabilities, axis=1).numpy()[0]
        confidence_score = probabilities[0, predicted_class_index]

        if confidence_score < 0.2:
            return {"label": "Incompréhensible", "response": "I don't understand your question."}

        predicted_class_name = label_mapping[str(predicted_class_index)]

        print(f"Confidence score: {confidence_score}, Predicted class: {predicted_class_name}")

        if predicted_class_name == "Add" or predicted_class_name == "Del":
            text = question.question
            doc = nlps(text)

            order_items = {}
            last_quantity = 1

            for ent in doc.ents:
                if ent.label_ == "qnt":
                    last_quantity = int(ent.text)
                elif ent.label_ == "food":
                    item_name = ent.text
                    order_items[item_name] = last_quantity
                    last_quantity = 1

        if predicted_class_name == "Add":
            for item, qty in order_items.items():
                existing_item = next((i for i in order['items'] if i['name'] == item), None)
                if existing_item:
                    existing_item['quantity'] += qty
                else:
                    order['items'].append({"name": item, "quantity": qty})
            response = "Items added to your order : " + ", ".join([f"{qty} {item}" for item, qty in order_items.items()]) + "." + " Thank you for your order!"

        elif predicted_class_name == "Del":
            new_items = []
            for existing_item in order['items']:
                item_name = existing_item['name']
                if item_name in order_items:
                    quantity_to_remove = order_items[item_name]
                    if existing_item['quantity'] > quantity_to_remove:
                        existing_item['quantity'] -= quantity_to_remove
                        new_items.append(existing_item)
                    # Si la quantité est égale ou inférieure, l'article est complètement supprimé
                else:
                    new_items.append(existing_item)
            order['items'] = new_items
            response = "Items removed from your order : " + ", ".join([f"{qty} {item}" for item, qty in order_items.items()]) + "." 

        else:
            response = random.choice(class_responses.get(predicted_class_name, ["No Answer Found"]))

        with open(order_file_path, 'w') as file:
            json.dump(order, file)

        return {"label": predicted_class_name, "response": response}
    except Exception as e:
        return {"error": str(e)}

