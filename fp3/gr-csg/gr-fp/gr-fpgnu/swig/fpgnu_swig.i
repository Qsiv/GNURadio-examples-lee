/* -*- c++ -*- */

#define FPGNU_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "fpgnu_swig_doc.i"

%{
#include "fpgnu/fpdata_sink.h"
#include "fpgnu/fpdata_source.h"
#include "fpgnu/fp_CandC.h"
%}


%include "fpgnu/fpdata_sink.h"
GR_SWIG_BLOCK_MAGIC2(fpgnu, fpdata_sink);
%include "fpgnu/fpdata_source.h"
GR_SWIG_BLOCK_MAGIC2(fpgnu, fpdata_source);
%include "fpgnu/fp_CandC.h"
GR_SWIG_BLOCK_MAGIC2(fpgnu, fp_CandC);
