from flask import Flask ,Blueprint, render_template
from blueprints.stop_pages import stop_pages
from blueprints.onboard_pages import onboard_pages

app = Flask(__name__)
app.register_blueprint(stop_pages, url_prefix='/stop')
app.register_blueprint(onboard_pages, url_prefix='/onboard')
app.config['DEBUG'] = True
app.config['TEMPLATES_AUTO_RELOAD'] = True


@app.route('/')
def hello_world():
    return 'Hello World!'


if __name__ == '__main__':
    app.run()
