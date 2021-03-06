import requests
from http import cookiejar
import os
import re
import sys
import time

COOKIE_FILE = "cookies.txt"
LINK = "https://d.pcs.baidu.com/file/87552e61aab1b4b42a49914163555073?fid=4096219213-250528-295309104555685&time=1493215895&rt=pr&sign=FDTAERVC-DCb740ccc5511e5e8fedcff06b081203-XykKamUE18t2JHjkeFOxP9NzmB4%3D&expires=8h&chkv=1&chkbd=1&chkpc=&dp-logid=2688621481016811391&dp-callid=0&r=268689083"
HEADERS = {
	"Content-type": "application/x-www-form-urlencoded",
	"User-Agent": "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:52.0) Gecko/20100101 Firefox/52.0",
	"Connection": "keep-alive",
	"Accept-Encoding": "gzip"
}

def read_cookies(filename):
	cookies = []
	need_add_header = True
	with open(filename, 'r') as f:
		for cookie in f.readlines():
			if cookie[0] == '#': 
				need_add_header = False
			cookie_splited = cookie.split("\t")
			if len(cookie_splited) == 6:
				cookie_splited.insert(4, str(int(time.time())))
				cookie = '\t'.join(cookie_splited)
			cookies.append(cookie)
	with open(filename, 'w') as f:
		if need_add_header: 
			f.write('# Netscape HTTP Cookie File\n')
		f.write(''.join(cookies))

	cookies = cookiejar.MozillaCookieJar(filename=filename)
	try:
		cookies.load()
		return cookies
	except Exception as e:
		print(e)
		exit(1)

cookies = read_cookies(filename = COOKIE_FILE)

def progress(total):
	width = 50
	def update(count):
		sys.stdout.write(' '*(width + 10) + '\r')
		sys.stdout.flush()

		prog = width * count // total
		sys.stdout.write('{0}/{1}: '.format(count, total))
		sys.stdout.write('#'*prog + '-'*(width-prog) + '\r')
		sys.stdout.flush()
	return update

def download():
	session = requests.session()
	session.headers = HEADERS
	session.cookies = cookies

	r = session.get(LINK, stream=True)

	#for key in r.headers: print(key+": "+r.headers[key])
	content_size = int(r.headers['content-length'])
	filename = re.search(r'filename=\"*(.*)\"*', r.headers['content-disposition']).group(1)

	print('File Name: {0}\nFile Size: {1}'.format(filename, content_size))

	with open(filename+'.size', 'w') as f:
		f.write(str(content_size))

	p = progress(content_size)
	count = 0
	with open(filename, 'wb') as f:
		for chunk in r.iter_content(chunk_size = 1024):
			if chunk:
				f.write(chunk)
				f.flush()
				count = count + len(chunk)
				p(count)
		if count >= content_size: 
			print('\nComplete!')
		else:
			print('\nIncomplete')

def retransmission(filename):
	read_size = os.path.getsize(filename)
	with open(filename+'.size', 'r') as f:
		content_size = int(f.read())

	HEADERS["Range"] = "bytes={0}-{1}".format(read_size, content_size)

	session = requests.session()
	session.headers = HEADERS
	session.cookies = cookies

	r = session.get(LINK, stream=True)

	p = progress(content_size)
	count = read_size
	with open(filename, 'ab') as f:
		for chunk in r.iter_content(chunk_size = 1024):
			if chunk:
				f.write(chunk)
				f.flush()
				count = count + len(chunk)
				p(count)
		if count >= content_size: 
			print('\nComplete!')
		else:
			print('\nIncomplete')

if __name__ == "__main__":
	if len(sys.argv) > 1:
		retransmission(sys.argv[1])
	else:
		download()


