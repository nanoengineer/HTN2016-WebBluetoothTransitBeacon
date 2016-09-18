from flask import Flask

from flask import render_template, jsonify
from helpers import helpers
from datetime import datetime, timedelta

app = Flask(__name__)
app.config['DEBUG'] = True
app.config['TEMPLATES_AUTO_RELOAD'] = True


@app.route('/')
def test():
    stop_num = "2"
    today = datetime.now() - timedelta(hours=4)
    routes = helpers.get_all_routes()
    stops = helpers.get_all_stops()
    for stop, times in stops.iteritems():
        stop_times = times['stop_times'].get('stop_' + stop_num)
        stop_times = [today.replace(hour=datetime.strptime(stop_time, '%I:%M%p').hour, minute=datetime.strptime(stop_time, '%I:%M%p').minute, second=0) for stop_time in stop_times]
        if filter(lambda x: x > today, stop_times):
            next = filter(lambda x: x > today, stop_times)[0]
            stop_times = filter(lambda x : x > next, stop_times)
        else:
            next = stop_times[0]
            stop_times = stop_times[1:]
        routes[stop.strip('route_')]['stops'] = {'all': stop_times, 'next': next}

    return render_template('stop.html', routes=routes)


@app.route("/ajax/stops/<route_num>")
def stops(route_num):
    stops = helpers.get_route_stops(route_num)
    stop_names = stops.get('stop_names')
    return jsonify(**stop_names)

if __name__ == "__main__":
    app.run()
