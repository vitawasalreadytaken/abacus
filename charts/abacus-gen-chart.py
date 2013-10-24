#!/usr/bin/python

import os, re, sqlite3, subprocess, sys

GP_CONF = gpConf = os.path.join(os.path.dirname(__file__), 'gnuplot')


def readConfig(path):
	text = file(path).read()
	# Remove the comments.
	text = re.sub('#.*\n', '\n', text)
	config = {}
	for line in text.split('\n'):
		if not line.strip():
			continue
		(key, val) = line.split('=', 1)
		config[key.strip()] = val.strip()
	return config


def getData(database, start, end):
	db = sqlite3.connect(database)
	cursor = db.cursor()
	sql = 'SELECT DATE(stamp) AS day, SUM(value) FROM counter %s GROUP BY day'
	if start and end:
		cursor.execute(sql % 'WHERE day BETWEEN ? AND ?', (start,end))
	else:
		cursor.execute(sql % '')
	return cursor.fetchall()


def feedGnuplot(data):
	data = '\n'.join([ '%s %s' % tup for tup in data ])
	gp = subprocess.Popen(['gnuplot'], stdin = subprocess.PIPE)
	gp.communicate(file(GP_CONF).read() + '\n' + data)


def main():
	config = readConfig('/etc/abacus.conf')
	try:
		(start, end) = sys.argv[1:3]
	except:
		(start, end) = (None, None)
	data = getData(config['database'], start, end)
	feedGnuplot(data)


if __name__ == '__main__':
	main()
