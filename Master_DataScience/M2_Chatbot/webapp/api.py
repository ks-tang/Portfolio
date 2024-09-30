from flask import Flask, render_template, request, jsonify
import requests
import json

app = Flask(__name__)

API_SERVING_URL = "http://api_serving:8080/predict/"

order_file_path = '../data/order.json'

@app.route('/ask', methods=['POST'])
def ask():
    data = request.get_json()
    question = data.get('question')
    if question:
        response = send_question_to_api_serving(question)
        return jsonify({"response": response})
    return jsonify({"error": "Question manquante"}), 400

def send_question_to_api_serving(question):
    try:
        payload = {"question": question}
        response = requests.post(API_SERVING_URL, json=payload)
        if response.status_code == 200:
            data = response.json()
            return data.get('response', 'Rephrase the request please.')
        else:
            return f"Erreur lors de la requête : {response.status_code}"
    except Exception as e:
        return f"Erreur lors de l'envoi de la question : {str(e)}"

@app.route('/get_order', methods=['GET'])
def get_order():
    try:
        with open(order_file_path, 'r') as file:  # Assurez-vous que le chemin est correct
            order = json.load(file)
            print(order)
        return jsonify(order)
    except Exception as e:
        app.logger.error(f"Erreur lors de l'obtention de la commande : {e}")
        return jsonify({"error": str(e)}), 500


@app.route('/', methods=['GET'])
def home():
    return render_template('index.html')

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
