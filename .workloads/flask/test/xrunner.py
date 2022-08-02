import time, json

from xh1 import encode, decode
from flask import Flask


app = Flask(__name__)

@app.route('/', methods=['GET'])
def hello_world():
    return 'Hello from Flask!!'

@app.route('/time', methods=['GET'])
def time_page():
    data_set = {"time":time.time()}
    json_dump = json.dumps(data_set)
    return json_dump

@app.route('/morse/<raw>', methods=['GET'])
def morse(raw):
	morse_code = {"A":"0x01", "B":"0x1000", "C":"0x1010", "D":"0x100", "E":"0x0", "F":"0x0010", "G":"0x110", "H":"0x0000", "I":"0x00", "J":"0x0111", "K":"0x101", "L":"0x0100", "M":"0x11", "N":"0x10", "O":"0x111", "P":"0x0110", "Q":"0x1101", "R":"0x010", "S":"0x000", "T":"0x1", "U":"0x001", "V":"0x0001", "W":"0x011", "X":"0x1001", "Y":"0x1011", "Z":"0x1100", "1":"0x01111", "2":"0x00111", "3":"0x00011", "4":"0x00001", "5":"0x00000", "6":"0x10000", "7":"0x11000", "8":"0x11100", "9":"0x11110", "0":"0x11111"}
	counter = 0
	add_morse_code = ''
	for x in range(len(raw)):
		text = raw[counter]
		encode_morsecode = morse_code[text.upper()]
		add_morse_code = add_morse_code + encode_morsecode[2:]
		counter += 1
	data = {"morse":add_morse_code}
	return data

@app.route('/xhash/<data>', methods=['GET'])
def xhash(data):
    a = data[:2]
    if a == str('e='):
        b = encode(data[2:])
        c = {"encoded":b}
    elif a == str('d='):
        b = decode(data[2:])
        c = {"deboded":b}
    return c


if __name__ == "__main__":
    app.run(debug = "True")

