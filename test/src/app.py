import time
import json
from urllib.request import urlopen

#username
with open("src/userinfo") as userinfo:
    username = userinfo.read().rstrip()
# DEBUG: for debuging (remove the hashtag to sea the results)
#print(username)

#getapi
# api source 'https://server.duinocoin.com/users/ + your_username'
api = "https://server.duinocoin.com/users/" + username
read = json.loads(urlopen(api).read())
# DEBUG: for debuging (remove the hashtag to sea the results)
#print(api)
#print(read)

#apimapping
result = read['result']
tempbalance = result['balance']
mainbalance = tempbalance['balance']
stake = tempbalance['stake_amount']
miners = result['miners']

# DEBUG: for debuging (remove the hashtag to sea the results)
#print(result)
#print(tempbalance)
#print(mainbalance)
#print(stake)
#print(miners)

#minerslist
minerlist = len(miners)


x = 0
while x <= minerlist:
    mins = str(x)
    minr = "miners[" + "\'" + str(x) + "\'" + "]"
    x += 1

miner0 = minr
print(miner0)

for i in minerlist:
    
'''
beta testing
miner0 = miners['0']
miner1 =
miner2 =
miner3 =
miner4 =
miner5 =
miner6 =
miner7 =
miner8 =
miner9 =
miner10 =
miner11 =
miner12 =
miner13 =
miner14 =
miner15 =
miner16 =
miner17 =
miner18 =
miner19 =
miner20 =
miner21 =
miner22 =
miner23 =
miner24 =
miner25 =
miner26 =
miner27 =
miner28 =
miner29 =
'''
