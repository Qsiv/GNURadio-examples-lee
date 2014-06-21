#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  untitled.py
#  
#  Copyright 2012 Balint Seeber <balint@crawfish>
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#  
#  

import sys, subprocess, time, tempfile, os
from optparse import OptionParser
from string import split

from gnuradio.eng_option import eng_option

_units = [
	(3, "k"),
	(6, "M"),
	(9, "G")
]

def _format_rate(rate):
	for (u1,s1), (u2,s2) in zip(_units, _units[1:]):
		n = pow(10, u1)
		if rate >= n and rate < pow(10, u2):
			r = rate % n
			if r > 0:
				return str(1.0*rate/n) + " " + s1
			else:
				return str(rate/n) + " " + s1
	return str(rate) + " "

def _sort(series, keys):
	if len(keys) == 0:
		return []
	key = keys[0]
	rev = False
	if key[0] == '-':
		key = key[1:]
		rev = True
	l = []
	for s in series:
		if s[key] not in l:
			l += [s[key]]
	l.sort()
	if rev:
		l.reverse()
	return [(key, l)] + _sort(series, keys[1:])

def _order(series, sort_list):
	if len(sort_list) == 0:
		return series
	(sort_key, sort_key_list) = sort_list[0]
	if len(sort_key_list) == 0:
		return []
	#print sort_key, sort_key_list
	l = []
	for s in series:
		if s[sort_key] == sort_key_list[0]:
			l += [s]
	#print l
	return _order(l, sort_list[1:]) + _order(series, [(sort_list[0][0], sort_list[0][1][1:])] + sort_list[1:])

def main():
	usage="%prog: [options]"
	parser = OptionParser(option_class=eng_option, usage=usage)
	parser.add_option("", "--id", type="string", help="device ID [default: %default]", default=None)
	parser.add_option("", "--sort", type="string", help="sort order [default: %default]", default="rate -spb -spp")
	(options, args) = parser.parse_args()
	
	series = []
	line = None
	if len(args) > 0:
		with open(args[0]) as f:
			lines = f.readlines()
	else:
		lines = sys.stdin.readlines()
	if lines is None or len(lines) == 0:
		return
	
	for line in lines:
		line = line.strip()
		if len(line) == 0:
			continue
		x = {'file':line}
		idx = line.find(".id_")
		if idx > -1:
			idx += 4
			idx2 = line.find("_", idx)
			x['id'] = line[idx:idx2]
			if options.id is None:
				options.id = x['id']
			elif options.id != x['id']:
				print "Different IDs:", options.id, x['id']
		idx = line.find("_rate_")
		if idx > -1:
			idx += 6
			idx2 = line.find("-", idx)
			x['rate'] = int(line[idx:idx2])
		idx = line.find("-spb_")
		if idx > -1:
			idx += 5
			idx2 = line.find("-", idx)
			x['spb'] = int(line[idx:idx2])
		idx = line.find("-spp_")
		if idx > -1:
			idx += 5
			idx2 = line.find(".", idx)
			x['spp'] = int(line[idx:idx2])
		print x
		series += [x]
	
	extra = []
	
	extra += ["set title \"Latency (" + options.id + ")\""]
	extra += ["set xlabel \"Latency (us)\""]
	extra += ["set ylabel \"Normalised success of on-time burst transmission\""]
	extra += ["set key right bottom"]
	extra += ["set grid"]
	
	gp = "plot "
	
	if len(extra) > 0:
		gp = "; \\\n".join(extra) + "; \\\n" + gp
	
	gp_series = ""
	
	sort_keys = options.sort.split()
	print sort_keys
	sorted_key_list = _sort(series, sort_keys)
	print sorted_key_list
	series = _order(series, sorted_key_list)
	#print series
	
	for s in series:
		if len(gp_series) > 0:
			gp_series += ", \\\n"
		gp_series += "\"" + s['file'] + "\" title \"" + ("%ssps, SPB %d, SPP %d" % (_format_rate(s['rate']), s['spb'], s['spp'])) + "\" with lp"
	
	gp += gp_series
	
	print "\nGNU Plot command:"
	
	tf = tempfile.NamedTemporaryFile(delete=False)
	tf.write(gp)
	tf.close()
	print tf.name
	
	print ""
	print gp
	
	gp_exec = [
		"gnuplot",
		"-p"
	]
	
	gp_exec += [tf.name]
	
	#p = None
	res = 0
	try:
		res = subprocess.call(gp_exec)
		
		# GNU Plot only reads 4096 characters from stdin
		#p = subprocess.Popen(gp_exec, stdin=subprocess.PIPE)	# , stdout=subprocess.PIPE, stderr=subprocess.PIPE, bufsize=16384
		#p.communicate(input=gp)
		
		#p.stdin.write(gp)
		#p.wait()
	except KeyboardInterrupt:
		print "Caught CTRL+C"
	#if p is not None and not p.returncode == 0:
	if res != 0:
		print "Failed to run subprocess (result: %d)" % (res)
	
	os.unlink(tf.name)
	
	return 0

if __name__ == '__main__':
	main()
