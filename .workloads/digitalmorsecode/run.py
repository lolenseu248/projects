from app import xencode, xdecode

text = 'helloworld'.upper()
method = 'v'

text2 = '0000','0','0100','0100','111','011','111','010','0100','100'


print(xencode(text, method))
print(xdecode(text2))