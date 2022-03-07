import time
import json
from urllib.request import urlopen

with open("src/userinfo") as userinfo:
    username = userinfo.read().rstrip()

api = "https://server.duinocoin.com/users/" + username
read = json.loads(urlopen(api).read())
retrive = read['result']
result = retrive['balance', 'miners']
a, b = result
print(a)
