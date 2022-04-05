import json
import random


#from xh1.json
xh1 = json.loads(open('src/xh1.json', 'r').read())

#src gen for data, fingerprint, and key
gen_data = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789'
gen_fp = '0123456789'
gen_key = '123456789'

#data, fingerprint, and key
gen_data_len = '00000000'
gen_fp_len = '00000000'
gen_key_len = '00000000'

#generating raw data, fingerprint, and key
def gendata():
    gendata = ''.join(random.choice(gen_data) for i in range(len(gen_data_len)))
    return gendata

def genfp():
    genfp = ''.join(random.choice(gen_fp) for i in range(len(gen_fp_len)))
    return genfp

class steaching():
    def __init__(self):
        self.banner = ''.join('-' for i in range(len(gen_fp_len)))
        self.rawdata = ''.join(f'{gendata()}\n' for i in range(len(gen_data_len)))
        self.rawfp =  ''.join(f'|{genfp()}|\n' for i in range(len(gen_fp_len)))
        self.rawkey = ''.join(random.choice(gen_key) for i in range(len(gen_key_len)))
        
steach = steaching()

rawdnfpnkey = f'data:\n{steach.rawdata}fingerprint:\n0{steach.banner}0\n{steach.rawfp}0{steach.banner}0\nkey:\n[{steach.rawkey}]\nkeylen: {len(gen_key_len)}'
#print(rawdnfpnkey) ##raw of data, fingerprint, and key

#generating xh1 data, fingerprint, and key
def gen_encode():
    global xh1_ram
    xh1_file = open('src/xhash.xh1', 'w')
    xh1_file.write('0x')
    xh1_ram = '0x'
    countdata = len(rawdnfpnkey)
    counter = 0
    while counter < countdata:
        procces = rawdnfpnkey[counter]
        strap = xh1[procces]
        xh1_file.write(strap)
        xh1_ram = xh1_ram + strap
        counter += 1
