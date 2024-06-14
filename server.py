from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/')
def home():
    return "Welcome to the Data Receiver Server!"

@app.route('/data', methods=['POST'])
def receive_data():
    data = request.get_json()
    if not data:
        return jsonify({'status': 'error', 'message': 'Missing JSON data'}), 400
    
    temperature = data.get('temperature')
    humidity = data.get('humidity')
    
    if temperature is None or humidity is None:
        return jsonify({'status': 'error', 'message': 'Missing temperature or humidity data'}), 400
    
    try:
        temperature = float(temperature)
        humidity = float(humidity)
    except ValueError:
        return jsonify({'status': 'error', 'message': 'Invalid data format'}), 400
    
    print(f"Received temperature: {temperature} C, humidity: {humidity} %")
    
    return jsonify({'status': 'success', 'message': 'Data received successfully'}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
