## requests simulator


import requests
import json

payload = {'row': 5, 'col': 7, 'state': 1, 'device_id': 2}
headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}

r = requests.post("http://localhost:5000/messages", data=json.dumps(payload), headers=headers)

print(r.text)

r = requests.get("http://localhost:5000/meow")

print(r.text)

# r = requests.get("http://localhost:5000/wowee/1/2/3")



print(r.text)