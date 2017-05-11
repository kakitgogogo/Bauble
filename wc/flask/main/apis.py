from . import main
from flask import render_template, url_for, request
import os
import json
import subprocess

@main.route('/api/addtext', methods=['GET', 'POST'])
def addtext():
	if request.method == 'GET':
		return render_template('404.html'), 404
	else:
		text = request.form['text']
		with open('wordcloud/text', 'a') as f:
			f.write('\n'+text)
		child = subprocess.Popen(['python3', 'wordcloud/gen.py'])
		child.wait()
		
		return render_template('index.html')

