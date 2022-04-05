from src import xh1
from src import gendnpfnkey

gendnpfnkey.gen_encode()
rawxh1 = gendnpfnkey.rawdnfpnkey

src1 = open('src/xhash.xh1', 'r').read()

src = xh1.encode(rawxh1)
if src == src1:
    print("ok")