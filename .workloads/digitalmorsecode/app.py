import time

morse_code = {"A":"0x01", "B":"0x1000", "C":"0x1010", "D":"0x100", "E":"0x0", "F":"0x0010", "G":"0x110", "H":"0x0000", "I":"0x00", "J":"0x0111", "K":"0x101", "L":"0x0100", "M":"0x11", "N":"0x10", "O":"0x111", "P":"0x0110", "Q":"0x1101", "R":"0x010", "S":"0x000", "T":"0x1", "U":"0x001", "V":"0x0001", "W":"0x011", "X":"0x1001", "Y":"0x1011", "Z":"0x1100", "1":"0x01111", "2":"0x00111", "3":"0x00011", "4":"0x00001", "5":"0x00000", "6":"0x10000", "7":"0x11000", "8":"0x11100", "9":"0x11110", "0":"0x11111"}


def encode():
    input_text = input("Enter a Text: ").upper().strip('\n')
    counter = 0
    for x in range(len(input_text)):
        text = input_text[counter]
        encode_morsecode = morse_code[text]
        print(encode_morsecode[2:])
        counter += 1

def decode():
    input_text = input("Enter a Text: ").strip('\n')



encode()
#decode()
