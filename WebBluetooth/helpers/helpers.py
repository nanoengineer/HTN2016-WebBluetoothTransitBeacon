# import pyrebase
# import requests
# config = {
#     'apiKey': "AIzaSyBfyz3Z_dqNHK7dn0JJT4GwqXP1R_xizWo",
#     'authDomain': "hackthenorth16-1532.firebaseapp.com",
#     'databaseURL': "https://hackthenorth16-1532.firebaseio.com",
#     'storageBucket': "hackthenorth16-1532.appspot.com"
# }
#
# firebase = pyrebase.initialize_app(config)
# db = firebase.database()
import requests

def get_all_routes():
    res = requests.get('https://hackthenorth16-1532.firebaseio.com/routes.json')
    return res.json()
    # return db.child("routes").get().val()

def get_all_stops():
    res = requests.get('https://hackthenorth16-1532.firebaseio.com/stops.json')
    return res.json()
    # return db.child("stops").get().val()

def get_route_stops(route_num):
    route_name = "route_" + route_num
    res = requests.get('https://hackthenorth16-1532.firebaseio.com/stops/' + route_name + '.json')
    return res.json()

if __name__ == '__main__':
    import pdb; pdb.set_trace()


