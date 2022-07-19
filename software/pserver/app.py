from flask import Flask

app = Flask(__name__)


@app.route('/v1/cc/hello')
def hello_world():  # put application's code here
    return 'Hello World!'

@app.route('/v1/cc/pair')
def pair():
    return 'json'

@app.route('/v1/cc/status')
def status():
    return 0

@app.route('/v1/cc/pair2')
def pair2():
    return 'json'

if __name__ == '__main__':
    app.run()
