#!/usr/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/cgardner/sandbox/gr-csg/grc/test_block/gr-testmod/python
export PATH=/home/cgardner/sandbox/gr-csg/grc/test_block/gr-testmod/build/python:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=/home/cgardner/sandbox/gr-csg/grc/test_block/gr-testmod/build/swig:$PYTHONPATH
/usr/bin/python /home/cgardner/sandbox/gr-csg/grc/test_block/gr-testmod/python/qa_testmod.py 
