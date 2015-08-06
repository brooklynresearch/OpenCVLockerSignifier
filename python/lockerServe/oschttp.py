# simple HTTP to OSC routing

import OSC
import logging
from flask import Flask, Response, jsonify, json, request

app = Flask(__name__)
c = OSC.OSCClient()
file_handler = logging.FileHandler('oschttp.log')
app.logger.addHandler(file_handler)
app.logger.setLevel(logging.INFO)

@app.route("/")
def hello():
  print "gott'd"
  oscmsg = OSC.OSCMessage()
  oscmsg.setAddress("/startup")
  oscmsg.append('HELLO')
  c.send(oscmsg)
  return "Hello World!"

@app.route("/meow")
@app.route("/meow/")
def goodbye():
  print "bye'd"
  return "Good bye!"

@app.route("/json")
def jsonny():
  print "json'd"
  list = [
    {'state': 0, 'row': 2, 'col': 3},
    {'param': 'ahoy', 'val': 2}
  ]

  return jsonify(results=list)

@app.route('/messages', methods=['GET', 'POST'])
def message_time():
  print "got in"
  print request.headers
  print request.data
  print request.json
  # app.logger.error("was there an error?")

  if request.headers['Content-Type'] == 'text/plain':
    return "Text Message: " + request.data

  elif request.headers['Content-Type'] == 'application/json':
    print request.json['row']
    print request.json['col']
    print request.json['state']
    print request.json['device_id']

    oscmsg = OSC.OSCMessage()
    oscmsg.setAddress("/startup")
    oscmsg.append(request.json['row'])
    oscmsg.append(request.json['col'])
    oscmsg.append(request.json['state'])
    oscmsg.append(request.json['device_id'])
    c.send(oscmsg)

    resp = Response(json.dumps(request.json), status=200, mimetype='application/json')
    # return "JSON Message: " + json.dumps(request.json)
    return resp

  elif request.headers['Content-Type'] == 'application/octet-stream':
    f = open('./binary', 'wb')
    f.write(request.data)
    f.close()
    return "Binary message written!"
  else:
    return "415 Unsupported Media Type ;)"
  return "crap"

@app.route('/wowee/<int:_row>/<int:_col>/<int:_state>', methods=['GET', 'POST'])
def wowter(_row, _col, _state):
  print _row
  oscmsg = OSC.OSCMessage()
  oscmsg.setAddress("/startup")
  oscmsg.append(_row)
  oscmsg.append(_col)
  oscmsg.append(_state)
  c.send(oscmsg)
  return "GOOD JOB@"

if __name__ == "__main__":
  c.connect(('127.0.0.1', 9998))
  print "jam"
  app.run(host='0.0.0.0', port=5000, debug=True)
