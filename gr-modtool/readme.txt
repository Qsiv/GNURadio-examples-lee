1) copy gr_modtool.py into working directory
2) run: python gr_modtool.py create <+Project_Name+> 

3) create test python code (use qa_projectname.py as example>
3a)Add to  gr-<+Project_Name+>/pythonpython directory 
3b) Add complie line to python/CMakeLists.txt
      GR_ADD_TEST(qa_+Project_Name+ ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_<+Project_Name+>.py)
   
4) Use gr_modtool to add new cc code
4a)  from project directory: gr_modtool.py add -t general <square_ff>
4a.I)  argument list: <>
4b.II) answer n to python and qa code 
        : Already placed it in python/ in the previous section
        
5) Edit the .h and .cc files
  5a) <ccmodule>_impl.cc edits:
  	modify private constructor (MIN_IN, MAX_IN...)
  	modify signal processing blocks

6) create build directory within project main directory
  6a) cd build
  6b) run:  cmake ../
  6c) sudo make
  
7) run sudo make test 
  7a) should run with no problems.
  7b) can run sudo ctest -V for verbose output

8) add to grc
  gr_modtool.py makexml square2_ff



Manual way
Copy gr-+Project_Name+-write-a-block to a new location (e.g. ~/src)
Rename the directory (e.g. gr-+Project_Name+)
Rename the project name in the top-level CMakeLists.txt file project(gr-+Project_Name+-write-a-block CXX C) ->  project(<newproject name> CXX C)
DO the following if starting from scratch
Remove the *.cc, *.h, *.xml and *.grc files
Remove all references to these files from all the CMakeLists.txt files

