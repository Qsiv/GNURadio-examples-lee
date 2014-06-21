#!/usr/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/python
export PATH=/home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/python:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=/home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig:$PYTHONPATH
/usr/bin/python /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/python/qa_fp_CandC.py 
