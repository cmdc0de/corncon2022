import datetime
import json
import random
import string
from dataclasses import dataclass
from flask import Flask, request, jsonify, Response
from flask_sqlalchemy import SQLAlchemy
import logging

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

@app.route('/v1/cc/reg', methods=['POST'])
def reg():
    rec = request.get_json()
    row = Registration(rec['badge_id'],rec['badge_name'], datetime.datetime.now())
    db.session.add(row)
    db.session.commit()
    return Response(json.dumps({'msg': 'success'}),status=200,mimetype='application/json')

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


class Pair(db.Model):
    pid = db.Column('pairing_id', db.Integer, primary_key=True, autoincrement=True)
    bid1 = db.Column('initating_badge_id', db.String(24))
    bname1 = db.Column('initating_badge_name', db.String(24))
    irand1 = db.Column('initating_random', db.Integer)
    color1 = db.Column('initating_badge_color', db.Integer)
    idateTime = db.Column('initating_date_time', db.DateTime)
    bid2 = db.Column('badge2_id', db.String(24))
    bname2 = db.Column('badge2_name', db.String(24))
    irand2 = db.Column('badge2_random', db.Integer)
    color2 = db.Column('badge2_badge_color', db.Integer)
    idateTime2 = db.Column('badge2_date_time', db.DateTime)
    pcode = db.Column('pairing_code', db.String(8))

    def __init__(self, bid, bn, rn1, c1):
        self.bid1 = bid
        self.bname1 = bn
        self.irand1 = rn1
        self.color1 = c1
        self.idateTime = datetime.datetime.now()
        letters = string.ascii_uppercase
        self.pcode = ''.join(random.choice(letters) for i in range(8))


@app.route('/v1/cc/pair', methods=['POST'])
def pair():
    r = request.get_json()
    row = Pair(r['badge_id'],r['badge_name'], r['rand'], r['badge_color'])
    db.session.add(row)
    db.session.commit()
    j = '{{"pcode":"{0}"}}'.format(row.pcode)
    return Response(j,status=201,mimetype='application/json')

@app.route('/v1/cc/status/<pc>', methods=['GET'])
def status(pc):
    sql = "select * from pair where pairing_code = '{0}' and badge2_date_time is not null".format(pc)
    rows = db.session.execute(sql).fetchone();
    if rows is None:
        return Response("{'r':0}",status=404,mimetype='application/json')
    else:
        return Response(json.dumps(dict(rows)), status=200, mimetype='application/json')

@app.route('/v1/cc/pair2/<pc>/<color>', methods=['POST'])
def pair2(pc, color):
    rec = request.get_json()
    sql2 = "update pair set badge2_id = '{0}', badge2_name = '{1}', badge2_random = {2}, badge2_badge_color = {3}, badge2_date_time = datetime('now') where pairing_code = '{4}'".format(
        rec['badge_id'],rec['badge_name'],rec['rand'],color, pc)
    db.session.execute(sql2)
    db.session.commit()
    sql = "select * from pair where pairing_code = '{0}' and badge2_date_time is not null".format(pc)
    rows = db.session.execute(sql).fetchone();
    if rows is None:
        return Response("{'r':0}",status=404,mimetype='application/json')
    else:
        return Response(json.dumps(dict(rows)), status=200, mimetype='application/json')

@app.route('/v1/cc/scores', methods=['GET'])
def get_scores():
    rows = db.session.execute("select * from high_score order by score desc limit 12")
    return Response(json.dumps([dict(r) for r in rows]),status=200,mimetype='application/json')

@app.route('/v1/cc/score', methods=['POST'])
def add_score():
    rec = request.get_json()
    row = HighScore(rec['badge_id'],rec['badge_name'], rec['score'], datetime.datetime.now())
    db.session.add(row)
    db.session.commit()
    return Response(json.dumps({'msg': 'success'}),status=200,mimetype='application/json')


if __name__ == '__main__':
    app.run()
