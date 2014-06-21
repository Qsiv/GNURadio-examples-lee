#!/usr/bin/env python
##################################################


from gnuradio import blks2
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.wxgui import forms
from grc_gnuradio import usrp as grc_usrp
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import wx

class FM_tx_FIFO(grc_wxgui.top_block_gui):

	def __init__(self):
		grc_wxgui.top_block_gui.__init__(self, title="Simple FM Transmitter FIFO input")

		##################################################
		# Variables
		##################################################
		self.samp_rate = samp_rate = 32000
		self.FM_freq = FM_freq = 107900000

		##################################################
		# Controls
		##################################################
		_FM_freq_sizer = wx.BoxSizer(wx.VERTICAL)
		self._FM_freq_text_box = forms.text_box(
			parent=self.GetWin(),
			sizer=_FM_freq_sizer,
			value=self.FM_freq,
			callback=self.set_FM_freq,
			label="FM Frequency",
			converter=forms.float_converter(),
			proportion=0,
		)
		self._FM_freq_slider = forms.slider(
			parent=self.GetWin(),
			sizer=_FM_freq_sizer,
			value=self.FM_freq,
			callback=self.set_FM_freq,
			minimum=87500000,
			maximum=108000000,
			num_steps=205,
			style=wx.SL_HORIZONTAL,
			cast=float,
			proportion=1,
		)
		self.Add(_FM_freq_sizer)

		##################################################
		# Blocks
		##################################################
		self.blks2_wfm_tx_0 = blks2.wfm_tx(
			audio_rate=samp_rate,
			quad_rate=640000,
			tau=75e-6,
			max_dev=75e3,
		)
		self.gr_file_source_0 = gr.file_source(gr.sizeof_short*1, "/home/mc/FM_demo/stream_32k.fifo", True)
		self.gr_multiply_const_vxx_0 = gr.multiply_const_vcc((32768, ))
		self.gr_multiply_const_vxx_1 = gr.multiply_const_vff((0.00003, ))
		self.gr_short_to_float_1 = gr.short_to_float()
		self.usrp_simple_sink_x_0 = grc_usrp.simple_sink_c(which=0, side="B")
		self.usrp_simple_sink_x_0.set_interp_rate(200)
		self.usrp_simple_sink_x_0.set_frequency(FM_freq, verbose=True)
		self.usrp_simple_sink_x_0.set_gain(0)
		self.usrp_simple_sink_x_0.set_enable(True)

		##################################################
		# Connections
		##################################################
		self.connect((self.gr_multiply_const_vxx_0, 0), (self.usrp_simple_sink_x_0, 0))
		self.connect((self.gr_short_to_float_1, 0), (self.gr_multiply_const_vxx_1, 0))
		self.connect((self.gr_file_source_0, 0), (self.gr_short_to_float_1, 0))
		self.connect((self.gr_multiply_const_vxx_1, 0), (self.blks2_wfm_tx_0, 0))
		self.connect((self.blks2_wfm_tx_0, 0), (self.gr_multiply_const_vxx_0, 0))

	def set_samp_rate(self, samp_rate):
		self.samp_rate = samp_rate

	def set_FM_freq(self, FM_freq):
		self.FM_freq = FM_freq
		self._FM_freq_slider.set_value(self.FM_freq)
		self._FM_freq_text_box.set_value(self.FM_freq)
		self.usrp_simple_sink_x_0.set_frequency(self.FM_freq)

if __name__ == '__main__':
	parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
	(options, args) = parser.parse_args()
	tb = FM_tx_FIFO()
	tb.Run(True)