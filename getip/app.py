import json
import urllib.request

apiipv4 = "https://api.ipify.org?format=json"
apiipv6 = "https://api64.ipify.org?format=json"
getip = json.loads(urllib.request.urlopen(apiipv4).read())
yourip = getip['ip']
print(yourip)
