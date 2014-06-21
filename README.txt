1) copy gr_modtool.py into working directory
2) run: python gr_modtool.py create <+Project_Name+> 

3) create test python code (use qa_projectname.py as example>
3a)Add to  gr-<+Project_Name+>/pythonpython directory 
3b) Add complie line to python/CMakeLists.txt
      GR_ADD_TEST(qa_+Project_Name+ ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_<+Project_Name+>.py)
   
4) Use gr_modtool to add new cc code
4a)  from project directory: python ../gr_modtool.py add -t general <square_ff>
4a.I)  argument list: <>
4b.II) answer n to python and qa code 
        : Already placed it in python/ in the previous section
        
5) Edit the .h and .cc files within the lib directory
  5a) <ccmodule>_impl.cc edits:
  	modify private constructor (MIN_IN, MAX_IN...)
  	modify signal processing blocks

6) create build directory within project main directory
  6a) cd build
  6b) sudo chmod -R 775 *
  6b) (NO SUDO) cmake ../
  6c) sudo make
  
7)sudo make test 
  7a) should run with no problems.
  7b) can run sudo ctest -V for verbose output
  7c) for errors, fix c,.py,.h and rerun 6c and 7a
  
8)sudo make install

9) add to grc
  1) from project directory python ../gr_modtool.py makexml <square_ff>
  2) add <?xml version="1.0"?> to xml file
  3)install the xml so grc can see it: 
    Set the GRC_BLOCKS_PATH environment variable to a path that contains your custom block wrapper. 
    The GRC_BLOCKS_PATH can contain multiple paths separated by colons: GRC_BLOCKS_PATH=/path/to/blocks1:/path/to/blocks2
    1) echo $GRC_BLOCKS_PATH
    temporary:
    2) export GRC_BLOCKS_PATH=${GRC_BLOCKS_PATH}:/home/cgardner/sandbox/gr-csg/grc/test_block/gr-testmod/grc
    permanent:
    2a) gedit ~/.bash_profile
    2b)append to GRC path:
    export GRC_BLOCKS_PATH=${GRC_BLOCKS_PATH}:/home/cgardner/sandbox/gr-csg/grc/test_block/gr-testmod/grc


 10) open NEW window and run gnuradio-companion
    a) add block and test...
    
    
SH notes
/usr/bin/sh <program>.sh


#test FP Data
python testdata.py -GNURadioTest -Capture -capture_mode 3 -decrate 15 -FlashXferSize 2
