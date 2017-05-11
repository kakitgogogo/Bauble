from . import main
from flask import render_template, url_for
import os

@main.errorhandler(404)
def no_such_process(e):
	return render_template('404.html'), 404

@main.route('/')
def index():
	return render_template('index.html')