from flask import Blueprint, render_template
from helpers import helpers

from datetime import datetime
import json

stop_pages = Blueprint('stop_pages', __name__, template_folder='templates')


@stop_pages.route("/")
def test():
    stop_num = "5"
    stop_dict = {}
    today = datetime.now()
    routes = helpers.get_all_routes()
    stops = helpers.get_all_stops()

    for stop, times in stops.iteritems():
        stop_times = [times['stop_times'].get('stop_' + stop_num)]
        # for stop_time in stop_times:
        #     datetime.strptime(stop_time, '%I:%M%p')


        stop_dict[stop.strip('route_')] = {'all': times['stop_times'].get('stop_' + stop_num)}

    # todo dump times based on rec'd stop
    return render_template('stop.html', routes=routes)

