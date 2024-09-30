import pandas as pd
import tensorflow as tf
from sklearn.model_selection import train_test_split
from transformers import BertTokenizer, TFBertForSequenceClassification
import json
import os
import pandas as pd

# Obtenez le répertoire du script courant
current_directory = os.path.dirname(__file__)

# Construisez le chemin vers le fichier 'intents.csv'
intents_path = os.path.join(current_directory, '..', 'data', 'intents.csv')

# Utilisez le chemin absolu pour charger le fichier CSV
data = pd.read_csv(intents_path)

# Encodage des labels
label_index = pd.factorize(data['Label'])[0]
label_mapping = dict(zip(range(len(data['Label'].unique())), data['Label'].unique()))

label_mapping_json = dict((str(i), label) for i, label in enumerate(data['Label'].unique()))
data['encoded_Label'] = pd.factorize(data['Label'])[0]

# Préparation des données
corpus = data['Question'].to_list()
labels = data['encoded_Label'].to_list()

# Utiliser len() pour obtenir le nombre d'éléments uniques
nombre_unique_labels = len(set(labels))

# Séparation des données en ensembles d'entraînement et de test
X_train, X_test, y_train, y_test = train_test_split(corpus, labels, test_size=0.2, random_state=0, stratify=labels)
# Initialisation du tokenizer
tokenizer = BertTokenizer.from_pretrained('bert-base-uncased')

# Fonction pour tokeniser les données
def data_tokenizer(data, tokenizer):
    return tokenizer(data, padding=True, truncation=True, max_length=512, return_tensors="tf")

# Tokenisation des données d'entraînement et de test
tokenized_X_train = data_tokenizer(X_train, tokenizer)
tokenized_X_test = data_tokenizer(X_test, tokenizer)

# Conversion des données tokenisées en dataset TensorFlow
train_dataset = tf.data.Dataset.from_tensor_slices((
    {key: val.numpy() for key, val in tokenized_X_train.items()},
    y_train
)).shuffle(1000).batch(16)

test_dataset = tf.data.Dataset.from_tensor_slices((
    {key: val.numpy() for key, val in tokenized_X_test.items()},
    y_test
)).batch(16)

# Chargement du modèle
model = TFBertForSequenceClassification.from_pretrained('bert-base-uncased', num_labels=nombre_unique_labels)

# Compilation du modèle
optimizer = tf.keras.optimizers.Adam(learning_rate=5e-5)
loss = tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True)
model.compile(optimizer=optimizer, loss=loss, metrics=['accuracy'])

# Initialisation de la variable pour conserver le meilleur modèle
best_val_accuracy = 0.0
best_model = None

epochs = 10

# Entraînement du modèle avec une boucle pour vérifier la précision de validation
for epoch in range(epochs):  # Remplacer par le nombre d'époques désiré
    print(f'Epoch {epoch + 1}/{epochs}')
    history = model.fit(train_dataset, epochs=1, validation_data=test_dataset)  # Entraînement pour une époque à chaque fois

    # Obtention de la précision de validation pour l'époque actuelle
    val_accuracy = history.history['val_accuracy'][0]
    
    # Vérification si le modèle actuel est le meilleur
    if val_accuracy > best_val_accuracy:
        best_val_accuracy = val_accuracy
        # Sauvegarde du modèle entier ou des poids, selon les besoins
        best_model = model.get_config()  # Pour l'architecture du modèle
        best_weights = model.get_weights()  # Pour les poids du modèle

model.set_weights(best_weights)

# Chemin vers le répertoire de sauvegarde des poids du modèle
model_weights_path = os.path.join(current_directory, '..', 'models', 'model_weights')

# Sauvegarde des poids du modèle
model.save_weights(model_weights_path)

# Chemin vers le fichier de mapping des labels
label_mapping_path = os.path.join(current_directory, '..', 'data', 'label_mapping.json')

# Sauvegarde du mapping des labels dans un fichier JSON
with open(label_mapping_path, 'w') as f:
    json.dump(label_mapping, f)