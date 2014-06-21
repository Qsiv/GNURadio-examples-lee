/* -*- c++ -*- */

#define TESTMOD_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "testmod_swig_doc.i"

%{
#include "testmod/square_ff.h"
#include "testmod/square2_ff.h"
%}


%include "testmod/square_ff.h"
GR_SWIG_BLOCK_MAGIC2(testmod, square_ff);
%include "testmod/square2_ff.h"
GR_SWIG_BLOCK_MAGIC2(testmod, square2_ff);
