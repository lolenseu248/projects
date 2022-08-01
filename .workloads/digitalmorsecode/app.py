import os, time
from typing import Counter
from unittest import addModuleCleanup

morse_code = {"A":"0x01", "B":"0x1000", "C":"0x1010", "D":"0x100", "E":"0x0", "F":"0x0010", "G":"0x110", "H":"0x0000", "I":"0x00", "J":"0x0111", "K":"0x101", "L":"0x0100", "M":"0x11", "N":"0x10", "O":"0x111", "P":"0x0110", "Q":"0x1101", "R":"0x010", "S":"0x000", "T":"0x1", "U":"0x001", "V":"0x0001", "W":"0x011", "X":"0x1001", "Y":"0x1011", "Z":"0x1100", "1":"0x01111", "2":"0x00111", "3":"0x00011", "4":"0x00001", "5":"0x00000", "6":"0x10000", "7":"0x11000", "8":"0x11100", "9":"0x11110", "0":"0x11111"}

def xencode(raw, method):
    counter = 0
    add_morse_code = ''
    for x in range(len(raw)):
        text = raw[counter]
        encode_morsecode = morse_code[text]
        if method == str('h'):
            add_morse_code = add_morse_code + encode_morsecode[2:]
        elif method == str('v'):
            add_morse_code = add_morse_code + encode_morsecode[2:] + '\n'
        counter += 1
    return add_morse_code

def xdecode(morse):
    counter = 0
    add_morse_code = ''
    for x in range(len(morse)):
        decode = morse[counter]
        decoded = [key for key, value in dict(morse_code).items() if value[2:] == decode]
        add_morse_code = add_morse_code + decoded[0]
        counter += 1
    return add_morse_code


def encode():
    input_text = input("Enter a Raw Text: ").upper().strip('\n')
    file_save = input("Enter a File Name to Save: ")
    save = open(f'{file_save}.txt', 'w')
    save.write('')

    counter = 0
    for x in range(len(input_text)):
        text = input_text[counter]
        encode_morsecode = morse_code[text]
        save.write(encode_morsecode[2:] + '\n')
        counter += 1


def decode():
    file_read = input("Enter a Saved File: ")
    file_save = input("Enter a File Name to Save: ")
    read = open(f'{file_read}').readlines()
    save = open(f'{file_save}.txt', 'w')

    counter = 0
    stripe = ''
    for x in range(len(read)):
        decode = read[counter].strip('\n')
        decoded = [key for key, value in dict(morse_code).items() if value[2:] == decode]
        stripe = stripe + decoded[0]
        counter += 1
    save.write(stripe)
    print(stripe)

def main():
    choice = input("Enter \"encode\" to Encode a Raw Text to MorseCode with no \"Spaces\" and \"decode\" to Decode a MorseCode to Raw Text:\n> ").lower()
    if choice == str('encode'):
        encode()
    elif choice == str('decode'):
        decode()
    else:
        print("Please Enter a Valid Choice!")
        time.sleep(5)
        os.system('clear')
        main()


#main()
