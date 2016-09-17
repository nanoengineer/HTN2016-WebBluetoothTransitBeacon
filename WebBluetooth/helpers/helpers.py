import pyrebase
import requests
config = {
    'apiKey': "AIzaSyBfyz3Z_dqNHK7dn0JJT4GwqXP1R_xizWo",
    'authDomain': "hackthenorth16-1532.firebaseapp.com",
    'databaseURL': "https://hackthenorth16-1532.firebaseio.com",
    'storageBucket': "hackthenorth16-1532.appspot.com"
}

firebase = pyrebase.initialize_app(config)
db = firebase.database()


def get_all_routes():
    return db.child("routes").get().val()

def get_all_stops():
    return db.child("stops").get().val()

if __name__ == '__main__':
    import pdb; pdb.set_trace()


