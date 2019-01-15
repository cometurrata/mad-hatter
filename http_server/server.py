from flask import Flask
app = Flask(__name__)

@app.route('/register', methods=['POST'])
def register():
    return 'REGISTER SUCCESS'

@app.route('/temperature-sensor', methods=['POST'])
def temperature_sensor():
    return 'Temperature sensor update received :)'

if __name__ == "__main__":
    app.run(host="0.0.0.0", port="80")