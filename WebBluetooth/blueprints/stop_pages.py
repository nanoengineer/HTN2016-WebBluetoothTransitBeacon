from flask import Blueprint, render_template, jsonify
from helpers import helpers
from datetime import datetime, timedelta
import json
import http

stop_pages = Blueprint('stop_pages', __name__, template_folder='templates')


@stop_pages.route("/")
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

@stop_pages.route("/ajax/stops/<route_num>")
def stops(route_num):
    stops = helpers.get_route_stops(route_num)
    stop_names = stops.get('stop_names')
    return jsonify(**stop_names)
