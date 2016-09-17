from flask import Flask ,Blueprint, render_template

onboard_pages = Blueprint('onboard_pages', __name__, template_folder='templates')


@onboard_pages.route("/")
def test():
    return render_template('onboard.html')