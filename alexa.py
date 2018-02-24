from firebase import firebase
from flask import Flask
from flask_ask import Ask,statement,question,session
import requests

app = Flask(__name__)
ask = Ask(app,"/")

url = 'https://alexaarduino-4ee6a.firebaseio.com/'
firebase = firebase.FirebaseApplication(url,None)

@ask.launch
def start():
    return question('hi there, do you want to know the temperature in this room?')

@ask.intent("YesIntent")
def yes():
    temperature = firebase.get('/temperature',None)
    if temperature >= 25:
        answer ='the temperature is {}...do you want to turn on the fan?'.format(temperature)
    else:
        answer ='the temperature is {}...do you want to turn off the fan?'.format(temperature)


    return question(answer)

@ask.intent("NoIntent")
def no():
    return statement('You are so handsome')

@ask.intent("TurnOn")
def on():
    post = firebase.patch('',{'power':1})
    return statement('of course')

@ask.intent("TurnOff")
def off():
    post = firebase.patch('',{'power':0})
    return statement('of course')

if __name__ == '__main__':
    app.run(debug=True)
