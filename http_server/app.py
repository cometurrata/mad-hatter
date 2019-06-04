from flask import Flask
from flask import request
import requests
app = Flask(__name__)

@app.route('/register/<node>', methods=['POST'])
def register(node):
    print("name : ", node , '\tip:', request.remote_addr )
    return 'REGISTER SUCCESS'

@app.route('/<node>/alive', methods=['POST'])
def alive(node):
    print("name : ", node , '\tip:', request.remote_addr )
    return 'REGISTER SUCCESS'

@app.route('/<node>', methods=['POST'])
def node(node):
    print("New request from " + node)
    if (node == "doorknock"):
        print(request.data)
        if request.data == b"event":
            print("Doorkock event")
            ip = request.remote_addr
            requests.get(url = "http://" + ip + "/write-pin", params={"pin":0, "value":0});
            pass
    elif (node == "clock"):
        if request.data == b"event":
            print("clock event")
            pass
    elif node == "musicbox":
        if request.data == b"event":
            print("Musicbox event")
            pass

    return 'OK'

if __name__ == "__main__":
    app.run(host="0.0.0.0", port="80")