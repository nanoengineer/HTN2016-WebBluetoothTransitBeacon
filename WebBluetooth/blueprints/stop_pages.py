from flask import Flask ,Blueprint, render_template

stop_pages = Blueprint('stop_pages', __name__, template_folder='templates')


@stop_pages.route("/")
def test():
    return render_template('stop.html')

