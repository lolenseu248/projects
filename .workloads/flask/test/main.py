from crypt import methods
import json, time


from flask import Flask

app = Flask(__name__)

@app.route('/', methods=['GET'])
def home_page():
    data_set = {"time":time.time()}
    json_dump = json.dumps(data_set)
    return json_dump

@app.route('/hello', methods=['GET'])
def error():
    return "Error 404"



if __name__ == "__main__":
    app.run(port=8024)
