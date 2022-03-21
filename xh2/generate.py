import os
import time
import json
import random
import urllib.request


#version
version = json.loads(open('../src/version.json', "r").read())


#source "https://www.ascii-code.com/"
#dictionary source
numbers = ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9')
letterslower = ('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z')
lettersupper = ('A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z')
specialchar = (' ', '~', ':', "'", '+', '[', '\\', '@', '^', '{', '%', '(', '-', '"', '*', '|', ',', '&', '<', '`', '}', '.', '_', '=', ']', '!', '>', ';', '?', '#', '$', ')', '/')
controlchar = ('NUL', 'SOH', 'STX', 'ETX', 'EOT', 'ENQ', 'ACK', 'BEL', 'BS', 'HT', 'LF', 'VT', 'FF', 'CR', 'SO', 'SI', 'DLE', 'DC1', 'DC2', 'DC3', 'DC4', 'NAK', 'SYN', 'ETB', 'CAN', 'EM', 'SUB', 'ESC', 'FS', 'GS', 'RS', 'US')
extendchar = ('Ç', 'ü', 'é', 'â', 'ä', 'à', 'å', 'ç', 'ê', 'ë', 'è', 'ï', 'î', 'ì', 'Ä', 'Å', 'É', '§', 'Æ', 'ô', 'ö', 'ò', 'û', 'ù', 'ÿ', 'Ö', 'Ü', '¢', '£', '¥', '₧', 'ƒ', 'á', 'í', 'ó', 'ú', 'ñ', 'Ñ', 'ª', 'º', '¿', '⌐', '¬', '½', '¼', '¡', '«', '»', '░', '▒', '▓', '│', '┤', '╡', '╢', '╖', '╕', '╣', '║', '╗', '╝', '╜', '╛', '┐', '└', '┴', '├', '├', '─', '┼', '╞', '╟', '╚', '╔', '╩', '╦', '╠', '═', '╬', '╧', '╨', '╤', '╥', '╙', '╘', '╒', '╓', '╫', '╪', '┘', '┌', '█', '▄', '▌', '▐', '▀', 'α', 'ß', 'Γ', 'π', 'Σ', 'σ', 'µ', 'τ', 'Φ', 'Θ', 'Ω','δ', '∞', 'φ', 'ε', '∩', '≡', '±', '≥', '≤', '⌠', '⌡', '÷', '≈', '°', '∙', '·', '√', 'ⁿ', '²', '■')
additional = ('\\n', '\\r', '\\t', '\\0', '\\xddd')
addiextrchar = ('©', '®',)


#steaching
printables = numbers + letterslower + lettersupper + specialchar
dictionary = controlchar + printables + extendchar + additional + addiextrchar


#generating key form dictionary
def genkey():
    global genkey
    key = open('key', "w")
    key.write('{\n')
    x = 0
    for i in range(len(dictionary)):
        rangen = ''.join((random.choice('qwertyuiopasdfghjklzcvbnmQWERTYUIOPASDFGHJKLZCVBNM1234567890') for i in range(4)))
        gendata = dictionary[x]
        firbase = '\"' + gendata +'\"' + ': ' + '\"' + rangen + '\",\n'
        secbase = '\"' + rangen +'\"' + ': ' + '\"' + gendata + '\",\n'
        key.write(firbase)
        key.write(secbase)
        #genkey = ''.join(firbase + secbase)
        #print(genkey)
        x += 1
    key.write('}')


#reading the generated key
def readkey():
    key = json.loads(open('key', "r").read())
    print(key)
    os.system('rm -rf key')


genkey()
#readkey()
print(len(dictionary))
