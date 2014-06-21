#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Mon Feb 11 13:33:49 2013
##################################################

from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.gr import firdes
from gnuradio.wxgui import forms
from gnuradio.wxgui import scopesink2
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import fpgnu_swig
import wx

class top_block(grc_wxgui.top_block_gui):

	def __init__(self):
		grc_wxgui.top_block_gui.__init__(self, title="Top Block")

		##################################################
		# Variables
		##################################################
		self.samp_rate = samp_rate = 32000
		self.remove_head = remove_head = 0

		##################################################
		# Blocks
		##################################################
		self._remove_head_chooser = forms.radio_buttons(
			parent=self.GetWin(),
			value=self.remove_head,
			callback=self.set_remove_head,
			label="Remove Head",
			choices=[0,1],
			labels=[],
			style=wx.RA_HORIZONTAL,
		)
		self.Add(self._remove_head_chooser)
		self.wxgui_scopesink2_0 = scopesink2.scope_sink_f(
			self.GetWin(),
			title="Scope Plot",
			sample_rate=samp_rate,
			v_scale=0,
			v_offset=0,
			t_scale=0,
			ac_couple=False,
			xy_mode=False,
			num_inputs=2,
			trig_mode=gr.gr_TRIG_MODE_AUTO,
			y_axis_label="Counts",
		)
		self.Add(self.wxgui_scopesink2_0.win)
		self.gr_udp_source_0 = gr.udp_source(gr.sizeof_short*1, "192.168.2.200", 9997, 590, False, True)
		self.gr_short_to_float_0_0 = gr.short_to_float(1, 1)
		self.gr_short_to_float_0 = gr.short_to_float(1, 1)
		self.gr_file_sink_0_1_0 = gr.file_sink(gr.sizeof_short*1, "/home/cgardner/sandbox/gr-csg/gr-fp/grc_testfp/test_fp_odd.bin")
		self.gr_file_sink_0_1_0.set_unbuffered(True)
		self.gr_file_sink_0_1 = gr.file_sink(gr.sizeof_short*1, "/home/cgardner/sandbox/gr-csg/gr-fp/grc_testfp/test_raw_even.bin")
		self.gr_file_sink_0_1.set_unbuffered(True)
		self.gr_deinterleave_0 = gr.deinterleave(gr.sizeof_short*1)
		self.fpgnu_fpdata_sink_0 = fpgnu_swig.fpdata_sink(remove_head)

		##################################################
		# Connections
		##################################################
		self.connect((self.gr_udp_source_0, 0), (self.fpgnu_fpdata_sink_0, 0))
		self.connect((self.gr_short_to_float_0, 0), (self.wxgui_scopesink2_0, 0))
		self.connect((self.gr_deinterleave_0, 1), (self.gr_file_sink_0_1_0, 0))
		self.connect((self.fpgnu_fpdata_sink_0, 0), (self.gr_deinterleave_0, 0))
		self.connect((self.gr_deinterleave_0, 0), (self.gr_file_sink_0_1, 0))
		self.connect((self.gr_short_to_float_0_0, 0), (self.wxgui_scopesink2_0, 1))
		self.connect((self.gr_deinterleave_0, 1), (self.gr_short_to_float_0_0, 0))
		self.connect((self.gr_deinterleave_0, 0), (self.gr_short_to_float_0, 0))

	def get_samp_rate(self):
		return self.samp_rate

	def set_samp_rate(self, samp_rate):
		self.samp_rate = samp_rate
		self.wxgui_scopesink2_0.set_sample_rate(self.samp_rate)

	def get_remove_head(self):
		return self.remove_head

	def set_remove_head(self, remove_head):
		self.remove_head = remove_head
		self._remove_head_chooser.set_value(self.remove_head)

if __name__ == '__main__':
	parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
	(options, args) = parser.parse_args()
	tb = top_block()
	tb.Run(True)

