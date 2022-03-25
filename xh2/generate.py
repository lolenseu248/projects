import os
import time
import json
import random
import urllib.request


#Files needed to run the program.
#version = json.loads(open('../src/version.json', "r").read())
rxh1 = json.loads(open('./src/xh1/rxh1.json', "r").read())
xh1 = json.loads(open('./src/xh1/xh1.json', "r").read())


#source "https://www.ascii-code.com/"
#dictionary source
numbers = ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')
letterslower = ('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z')
lettersupper = ('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z')
specialchar = (' ', '~', ':', "'", '+', '[', '@', '^', '{', '%', '(', '-', '\\"', '*', '|', ',', '&', '<', '`', '}', '.', '_', '=', ']', '!', '>', ';', '?', '#', '$', ')', '/', '\\' + '\\')
controlchar = ('NUL', 'SOH', 'STX', 'ETX', 'EOT', 'ENQ', 'ACK', 'BEL', 'BS', 'HT', 'LF', 'VT', 'FF', 'CR', 'SO', 'SI', 'DLE', 'DC1', 'DC2', 'DC3', 'DC4', 'NAK', 'SYN', 'ETB', 'CAN', 'EM', 'SUB', 'ESC', 'FS', 'GS', 'RS', 'US')
extendchar = ('Ç', 'ü', 'é', 'â', 'ä', 'à', 'å', 'ç', 'ê', 'ë', 'è', 'ï', 'î', 'ì', 'Ä', 'Å', 'É', '§', 'Æ', 'ô', 'ö', 'ò', 'û', 'ù', 'ÿ', 'Ö', 'Ü', '¢', '£', '¥', '₧', 'ƒ', 'á', 'í', 'ó', 'ú', 'ñ', 'Ñ', 'ª', 'º', '¿', '⌐', '¬', '½', '¼', '¡', '«', '»', '░', '▒', '▓', '│', '┤', '╡', '╢', '╖', '╕', '╣', '║', '╗', '╝', '╜', '╛', '┐', '└', '┴', '├', '─', '┼', '╞', '╟', '╚', '╔', '╩', '╦', '╠', '═', '╬', '╧', '╨', '╤', '╥', '╙', '╘', '╒', '╓', '╫', '╪', '┘', '┌', '█', '▄', '▌', '▐', '▀', 'α', 'ß', 'Γ', 'π', 'Σ', 'σ', 'µ', 'τ', 'Φ', 'Θ', 'Ω','δ', '∞', 'φ', 'ε', '∩', '≡', '±', '≥', '≤', '⌠', '⌡', '÷', '≈', '°', '∙', '·', '√', 'ⁿ', '²', '■')
additional = ('\\n', '\\r', '\\t', '\\' + '\\a', '\\' + '\\0')
addiextrchar = ('©', '®',)


#steaching
printables = letterslower + lettersupper + numbers + specialchar
dictionary = controlchar + extendchar + printables + additional + addiextrchar
#print(len(dictionary)) ##debuging for total characters 


#generating key form dictionary
def genkey():
    global genkey
    key = open('key.json', "w")
    key.write('{\n')


    #generating json format key
    x = 0
    for i in range(len(dictionary)):
        rangen = ''.join((random.choice('qwertyuiopasdfghjklzcvbnmQWERTYUIOPASDFGHJKLZCVBNM1234567890') for i in range(4)))
        gendata = dictionary[x]
        firbase = '\"' + gendata + '\"' + ':' + '\"' + rangen + '\",\n'
        secbase = '\"' + rangen + '\"' + ':' + '\"' + gendata + '\",\n'
        key.write(firbase)
        key.write(secbase)
        x += 1


    key.write('\"' + "xh1" + '\"' + ':' + '\"' + "ok" + '\"\n')
    key.write('}')


#reading the generated key and generating xh1 format of the key
def readkey():
    global key
    key = json.loads(open('key.json', "r").read())
    data = str(key)

    #generating xh1 format of the key
    genfilename = ''.join((random.choice('qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890') for i in range(8)))
    filename = genfilename + '.key'
    xh1key = open(filename, 'w')
    xh1key.write('0x')

    #starting the writing the xh1key
    counter = 0
    for i in range(len(data)):
        procces = data[counter]
        strap = xh1[procces]
        xh1key.write(strap[2:])
        counter += 1
        

genkey()
readkey()