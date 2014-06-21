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

import subprocess, time
from optparse import OptionParser
from string import split

from gnuradio.eng_option import eng_option

def launch_test(args="", rate=None, spb=None, spp=0, prefix="", suffix="", extra=[], verbose=False):
	responder = [
		"./responder"
	]
	if args is not None and len(args) > 0:
		responder += ["--args=\"" + args + "\""]
	if rate is not None and rate > 0:
		responder += ["--rate=%f" % (rate)]
	if spb is not None and spb > 0:
		responder += ["--spb=%d" % (spb)]
	if spp is not None and spp > 0:
		responder += ["--spp=%d" % (spp)]
	if prefix is not None and len(prefix) > 0:
		responder += ["--stats-file-prefix=" + prefix]
	if suffix is not None and len(suffix) > 0:
		responder += ["--stats-file-suffix=" + suffix]
	if extra is not None:
		responder += extra
	if verbose:
		print "==> Executing:", " ".join(responder)
	try:
		res = subprocess.call(responder)
	except KeyboardInterrupt:
		print "==> Caught CTRL+C"
		return None
	if not res == 0:
		print "Failed to run responder (result: %d)" % (res)
	return res

def main():
	usage="%prog: [options] [extra arguments]"
	parser = OptionParser(option_class=eng_option, usage=usage)
	parser.add_option("", "--rates", type="string", help="sample rates (Msps) [default: %default]", default=".25 1 4 8 25")
	parser.add_option("", "--spbs", type="string", help="samples per block [default: %default]", default="32 64 256 1024")
	parser.add_option("", "--spps", type="string", help="samples per packet (0: driver default) [default: %default]", default="0 64 128 256 512")
	parser.add_option("", "--args", type="string", help="UHD device arguments [default: %default]", default=None)
	parser.add_option("", "--prefix", type="string", help="Stats filename prefix [default: %default]", default=None)
	parser.add_option("", "--suffix", type="string", help="Stats filename suffix [default: %default]", default=None)
	parser.add_option("", "--pause", action="store_true", help="pause between tests [default=%default]", default=False)
	parser.add_option("", "--interactive", action="store_true", help="enable prompts within test [default=%default]", default=False)
	parser.add_option("", "--wait", type="float", help="time to wait between tests (seconds) [default=%default]", default=0.0)
    
	(options, args) = parser.parse_args()
    
	rates = map(lambda x: float(x)*1e6, split(options.rates))
	spbs = map(int, split(options.spbs))
	spps = map(int, split(options.spps))
    
	#prefix, suffix = "", ""
	#if len(args) > 0:
	#	prefix = args[0]
	#	print "Prefix:", prefix
	#if len(args) > 1:
	#	suffix = args[1]
	#	print "Suffix:", suffix
	
	extra_args = {
		"adjust-simulation-rate": None,
		"time-mul":		"1e6",
		"test-success": 5,
		"simulate":		1000,
		"iterations":	1000,
		"delay-min":	"50e-6",
		"delay-max":	"5e-3",
		"delay-step":	"50e-6"
	}
	
	if options.interactive is not True:
		extra_args["batch-mode"] = None
	
	for arg in args:
		if len(arg) > 2 and arg[0:2] == "--":
			arg = arg[2:]
		idx = arg.find('=')
		if idx == -1:
			extra_args[arg] = None
		else:
			extra_args[arg[0:idx]] = arg[idx+1:]
	
	def _format_arg(d, k):
		a =  "--" + str(k)
		if d[k] is not None:
			a += "=" + str(d[k])
		return a
	
	extra = map(lambda x: _format_arg(extra_args, x), extra_args)
	print "\n".join(map(lambda x: str(x) + " = " + str(extra_args[x]), extra_args.keys()))
	
	try:
		print "\nPress ENTER to start..."
		raw_input()
	except KeyboardInterrupt:
		print "Aborted"
		return 1
	
	count = 0
	results = {}
	
	try:
		for rate in rates:
			results_rates = results[rate] = {}
			for spb in spbs:
				results_spbs = results_rates[spb] = {}
				for spp in spps:
					if count > 0:
						if options.pause:
							print "Press ENTER to begin next test..."
							raw_input()
						elif options.wait > 0:
							time.sleep(options.wait)
					res = launch_test(options.args, rate, spb, spp, options.prefix, options.suffix, extra)
					count += 1
					if res is None:
						raise Exception
					results_spbs[spp] = res
	except:
		pass
	
	for rate in results.keys():
		results_rates = results[rate]
		for spb in results_rates.keys():
			results_spbs = results_rates[spb]
			for spp in results_spbs.keys():
				res = results_spbs[spp]
				print rate, spb, spp, res
	
	return 0

if __name__ == '__main__':
	main()
