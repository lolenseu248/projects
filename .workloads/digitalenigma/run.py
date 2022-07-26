import time
import json

#files
readencodefile = open('file.txt', 'r').read().upper().rstrip('\n')
plugboardconfig = json.load(open('plugboardconfig.json'))
print(plugboardconfig)
print(readencodefile)


#settings in plugboard
counter = 0
for x in range(len(readencodefile)):
    z = readencodefile[counter]
    c = plugboardconfig[z]
    if c == str(0):
        c = [key for key, value in dict(plugboardconfig).items() if value == c]
    counter += 1
    print(c)
    #print(z)

#rotttors
counter_a = 0
counter_b = 0
counter_c = 0
rotate_counter_a = 3
rotate_counter_b = 3
rotate_counter_c = 3

while True:
    counter_c += 1
    if counter_c == int(27):
        counter_c = 0
    for x in range(26):
        counter_b += 1
        if counter_b == int(27):
            counter_b = 0
        for x in range(26):
            counter_a += 1
            if counter_a == int(27):
                counter_a = 1
            print(counter_a, counter_b, counter_c)
            time.sleep(0.201)

"""
while True:
    if counter_a >= 26:
        counter_b += 1
        counter_a == 1

        if counter_b >= 26:
            counter_c += 1
            counter_b == 1

            if counter_c >= 26:
                counter_c == 1
"""
