import json

#from xh1.json
xh1 = json.loads(open('xh1.json', 'r').read())

#encode
def encode(data):
    encodexh1 = '0x'
    countdata = len(data)
    counter = 0
    while counter < countdata:
        procces = data[counter]
        strap = xh1[procces]
        encodexh1 = encodexh1 + strap[2:]
        counter += 1
    return encodexh1

#decode
def decode(data):
    decodexh1 = ''
    numsdata = data[2:]
    countdata = len(numsdata) / 2
    counter1 = 0
    counter2 = 0
    counter3 = 2
    while counter1 < countdata:
        procces = '0x' + numsdata[counter2:counter3]
        strap = [key for key, value in dict(xh1).items() if value == procces]
        decodexh1 = decodexh1 + strap[0]
        counter1 += 1
        counter2 += 2
        counter3 += 2
    return decodexh1
