import datetime
import json
from dataclasses import dataclass
from flask import Flask, request, jsonify
from flask_sqlalchemy import SQLAlchemy

app = Flask(__name__)
app.config ['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///static/my.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)

class Registration(db.Model):
    badge_id = db.Column('badge_id', db.String(24), primary_key = True)
    badge_name = db.Column(db.String(32))
    registration_time = db.Column(db.DateTime)

    def __init__(self, id, name, d):
        self.badge_id = id
        self.badge_name = name
        self.registration_time = d

@dataclass
class HighScore(db.Model):
    seq_id = db.Column('seq_id', db.Integer, primary_key=True, autoincrement=True)
    badge_id = db.Column('badge_id', db.String(24))
    badge_name = db.Column('badge_name', db.String(24))
    score = db.Column('score', db.Integer)
    score_create_date = db.Column(db.DateTime)

    def __init__(self, id, n, s, d):
        self.badge_id = id
        self.badge_name = n
        self.score = s
        self.score_create_date = d

@app.route('/v1/cc/hello')
def hello_world():  # put application's code here
    return 'Hello World!'

@app.route('/v1/cc/scores', methods=['GET'])
def get_scores():
    #rows = db.session.query(HighScore).order_by(HighScore.score).limit(10).all()
    rows = db.session.execute("select * from high_score order by score desc limit 10")
    return json.dumps([dict(r) for r in rows])

@app.route('/v1/cc/score', methods=['POST'])
def add_score():
    rec = request.get_json()
    row = HighScore(rec['badge_id'],rec['badge_name'], rec['score'], datetime.datetime.now())
    db.session.add(row)
    db.session.commit()
    return json.dumps({'msg': 'success'})

@app.route('/v1/cc/register', methods=['POST'])
def register():
    record = request.get_json()
    row = Registration(record['badge_id'],record['badge_name'], datetime.datetime.now())
    db.session.add(row)
    db.session.commit()
    return json.dumps({'msg': 'success'})

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
