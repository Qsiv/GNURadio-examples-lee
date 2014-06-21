#!/usr/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/lib
export PATH=/home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/lib:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-fpgnu 
