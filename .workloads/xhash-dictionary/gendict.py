#!/bin/python

import os
import time
import json


#results, runtime, and logs

#results
class results():
    rlo = '[ load! ]' #loading result
    rok = '[  ok!  ]' #ok result
    rer = '[  er!  ]' #error result
    rgo = '[ good! ]' #googd result
    rba = '[ bad!  ]' #bad result

r = results()

#runtime
def runtime():
    time.sleep(.5)

#making .logs
def makelogs():
    try:
        os.system('mkdir ./.logs')
        print(f"{r.rok} makelogs.")
    except:
        print(f"{r.rer} makelogs.")
        pass

#remove .logs
def removelogs():
    try:
        os.system('rm -rf ./.logs')
        print(f"{r.rok} removelogs.")
    except:
        print(f"{r.rer} removelogs.")
        pass

#all starting run fuctions here!

#runfuc
def runfuc():
    print(f"{r.rlo} Generating Logs...")
    runtime()
    makelogs()

    print(f"{r.rlo} Reading Srclist...")
    runtime()
    readsrc()
    
    print(f"{r.rlo} Generating List...")
    runtime()
    genptlist()

    print(f"{r.rlo} Generating Numbers...")
    runtime()
    gennum1()
    gennum2()

    print(f"{r.rlo} Generating Dictionary...")
    runtime()
    gendict()

    print(f"{r.rlo} Removing Logs...")
    runtime()
    removelogs()

#all src fuctions run here!

#read the text
def readsrc():
    print(f"{r.rlo} Reading Control...")
    runtime()

    print(f"{r.rlo} Reading Shortcut...")
    runtime()

    print(f"{r.rlo} Reading Printed...")
    runtime()
    try:
        global ptext, pkeys
        ptext = open('./gendictsrc/printed/text', 'r').read().rstrip('\n')
        pkeys = open('./gendictsrc/printed/keys', 'r').read().rstrip('\n')
        print(f"{r.rok} readprinted.")
    except:
        print(f"{r.rer} readprinted.")
        pass

#gen ptable list/tuples
def genptlist():
    runtime()
    try:
        count1 = 0
        genptllog = open('./.logs/.genptllog', 'w')
        genptllog.write('(')
        for i in range(len(ptext)):
            if ptext[count1] == '\"':
                genptllog.write('\"\\' + str(ptext[count1]) + '\", ')
            else:
                genptllog.write('\"' + str(ptext[count1]) + '\", ')

            count1 += 1

        genptllog.write('\"genptllogok\")')
        print(f"{r.rok} genptlist.")
    except:
        print(f"{r.rer} genptlist.")
        pass

#for printed text

#use letter as a numbering
def gennum1():
    runtime()
    try:
        count1, count2 = 0, 0
        gennum1log = open('./.logs/.gennum1log', 'w')
        gennum1log.write('{\n')
        for i in range(len(pkeys)):
            gennum1log.write('\t\"' + str(count2) + '\":\"' + str(pkeys[count1]) + '\",\n')

            count1 += 1
            count2 += 1

        gennum1log.write('\t\"gennum1\":\"ok!\"\n}')
        print(f"{r.rok} gennum1.")
    except:
        print(f"{r.rer} gennum1.")
        pass

def gennum2():
    runtime()
    try:
        count1, count2, count3 = 1, 0, 1
        countlimit = 10
        gennum2log = open('./.logs/.gennum2log', 'w')
        gennum2log.write('{\n\t\"0\":\"00",\n')
        for i in range(len(ptext)):
            if count1 >= countlimit:
                count3 = 0
                count2 += 1
                gennum2log.write('\t\"' + str(count1) + '\":\"' + str(pkeys[count2]) +  str(pkeys[count3]) + '\",\n')
                countlimit += 10
            else:
                gennum2log.write('\t\"' + str(count1) + '\":\"' + str(pkeys[count2]) +  str(pkeys[count3]) + '\",\n')

            count1 += 1
            count3 += 1

        gennum2log.write('\t\"gennum2\":\"ok!\"\n}')
        print(f"{r.rok} gennum2.")   
    except:
        print(f"{r.rer} gennum2.")
        pass


#making a dictionay form text and keys
def gendict():
    runtime()
    try:
        gennum = json.loads(open('./.logs/.gennum2log', 'r').read())
        count1, count2, = 0, 1
        gendictlog = open('./dictionary/xh1/xh1.json', 'w')
        gendictlog.write('{\n')
        for i in range(len(ptext)):
            if ptext[count1] == '\"':
                gendictlog.write('\t\"\\' + str(ptext[count1]) + '\":\"0x' + str(gennum[str(count2)]) + '\",\n')
            elif ptext[count1] == '\\':
                gendictlog.write('\t\"\\' + str(ptext[count1]) + '\":\"0x' + str(gennum[str(count2)]) + '\",\n')
            else:
                gendictlog.write('\t\"' + str(ptext[count1]) + '\":\"0x' + str(gennum[str(count2)]) + '\",\n')

            count1 += 1
            count2 += 1
        
        gendictlog.write('\t\"xh1\":\"ok\"\n}')
        print(f"{r.rok} gendict.")
    except:
        print(f"{r.rer} gendict.")
        pass


runfuc()
