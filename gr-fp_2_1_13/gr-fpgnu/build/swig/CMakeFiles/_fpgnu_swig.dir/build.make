# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build

# Include any dependencies generated for this target.
include swig/CMakeFiles/_fpgnu_swig.dir/depend.make

# Include the progress variables for this target.
include swig/CMakeFiles/_fpgnu_swig.dir/progress.make

# Include the compile flags for this target's objects.
include swig/CMakeFiles/_fpgnu_swig.dir/flags.make

swig/fpgnu_swigPYTHON_wrap.cxx: ../swig/fpgnu_swig.i
swig/fpgnu_swigPYTHON_wrap.cxx: /usr/include/gruel/swig/gruel_common.i
swig/fpgnu_swigPYTHON_wrap.cxx: /usr/include/gnuradio/swig/gnuradio.i
swig/fpgnu_swigPYTHON_wrap.cxx: swig/fpgnu_swig_doc.i
swig/fpgnu_swigPYTHON_wrap.cxx: /usr/include/gnuradio/swig/gr_shared_ptr.i
swig/fpgnu_swigPYTHON_wrap.cxx: swig/fpgnu_swig.tag
swig/fpgnu_swigPYTHON_wrap.cxx: ../swig/fpgnu_swig.i
	$(CMAKE_COMMAND) -E cmake_progress_report /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Swig source"
	cd /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig && /usr/bin/cmake -E make_directory /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig
	cd /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig && /usr/bin/swig -python -fvirtual -modern -keyword -w511 -module fpgnu_swig -I/usr/include/gnuradio/swig -I/usr/include/gruel/swig -I/usr/include/python2.7 -I/usr/include/python2.7 -I/home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/swig -I/home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig -outdir /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig -c++ -I/home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/include -I/usr/include -I/usr/include -I/usr/include -I/usr/include/gnuradio -I/usr/include/gnuradio/swig -I/usr/include/gruel/swig -I/usr/include/python2.7 -I/usr/include/python2.7 -I/home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/swig -I/home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig -o /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig/fpgnu_swigPYTHON_wrap.cxx /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/swig/fpgnu_swig.i

swig/fpgnu_swig.py: swig/fpgnu_swigPYTHON_wrap.cxx

swig/fpgnu_swig_doc.i: swig/fpgnu_swig_doc_swig_docs/xml/index.xml
	$(CMAKE_COMMAND) -E cmake_progress_report /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating fpgnu_swig_doc.i"
	cd /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/docs/doxygen && /usr/bin/python -B /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/docs/doxygen/swig_doc.py /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig/fpgnu_swig_doc_swig_docs/xml /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig/fpgnu_swig_doc.i

swig/fpgnu_swig.tag: swig/fpgnu_swig_doc.i
swig/fpgnu_swig.tag: swig/_fpgnu_swig_swig_tag
	$(CMAKE_COMMAND) -E cmake_progress_report /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating fpgnu_swig.tag"
	cd /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig && ./_fpgnu_swig_swig_tag
	cd /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig && /usr/bin/cmake -E touch /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig/fpgnu_swig.tag

swig/fpgnu_swig_doc_swig_docs/xml/index.xml: swig/_fpgnu_swig_doc_tag
	$(CMAKE_COMMAND) -E cmake_progress_report /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating doxygen xml for fpgnu_swig_doc docs"
	cd /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig && ./_fpgnu_swig_doc_tag
	cd /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig && /usr/bin/doxygen /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig/fpgnu_swig_doc_swig_docs/Doxyfile

swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o: swig/CMakeFiles/_fpgnu_swig.dir/flags.make
swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o: swig/fpgnu_swigPYTHON_wrap.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o"
	cd /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig && /usr/lib64/ccache/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o -c /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig/fpgnu_swigPYTHON_wrap.cxx

swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.i"
	cd /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig/fpgnu_swigPYTHON_wrap.cxx > CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.i

swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.s"
	cd /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig && /usr/lib64/ccache/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig/fpgnu_swigPYTHON_wrap.cxx -o CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.s

swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o.requires:
.PHONY : swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o.requires

swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o.provides: swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o.requires
	$(MAKE) -f swig/CMakeFiles/_fpgnu_swig.dir/build.make swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o.provides.build
.PHONY : swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o.provides

swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o.provides.build: swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o

# Object files for target _fpgnu_swig
_fpgnu_swig_OBJECTS = \
"CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o"

# External object files for target _fpgnu_swig
_fpgnu_swig_EXTERNAL_OBJECTS =

swig/_fpgnu_swig.so: swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o
swig/_fpgnu_swig.so: swig/CMakeFiles/_fpgnu_swig.dir/build.make
swig/_fpgnu_swig.so: /usr/lib64/libpython2.7.so
swig/_fpgnu_swig.so: lib/libgnuradio-fpgnu.so
swig/_fpgnu_swig.so: /usr/lib64/libboost_filesystem-mt.so
swig/_fpgnu_swig.so: /usr/lib64/libboost_system-mt.so
swig/_fpgnu_swig.so: /lib64/libgruel.so
swig/_fpgnu_swig.so: /lib64/libgnuradio-core.so
swig/_fpgnu_swig.so: swig/CMakeFiles/_fpgnu_swig.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared module _fpgnu_swig.so"
	cd /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/_fpgnu_swig.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
swig/CMakeFiles/_fpgnu_swig.dir/build: swig/_fpgnu_swig.so
.PHONY : swig/CMakeFiles/_fpgnu_swig.dir/build

swig/CMakeFiles/_fpgnu_swig.dir/requires: swig/CMakeFiles/_fpgnu_swig.dir/fpgnu_swigPYTHON_wrap.cxx.o.requires
.PHONY : swig/CMakeFiles/_fpgnu_swig.dir/requires

swig/CMakeFiles/_fpgnu_swig.dir/clean:
	cd /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig && $(CMAKE_COMMAND) -P CMakeFiles/_fpgnu_swig.dir/cmake_clean.cmake
.PHONY : swig/CMakeFiles/_fpgnu_swig.dir/clean

swig/CMakeFiles/_fpgnu_swig.dir/depend: swig/fpgnu_swigPYTHON_wrap.cxx
swig/CMakeFiles/_fpgnu_swig.dir/depend: swig/fpgnu_swig.py
swig/CMakeFiles/_fpgnu_swig.dir/depend: swig/fpgnu_swig_doc.i
swig/CMakeFiles/_fpgnu_swig.dir/depend: swig/fpgnu_swig.tag
swig/CMakeFiles/_fpgnu_swig.dir/depend: swig/fpgnu_swig_doc_swig_docs/xml/index.xml
	cd /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/swig /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig /home/cgardner/sandbox/gr-csg/gr-fp/gr-fpgnu/build/swig/CMakeFiles/_fpgnu_swig.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : swig/CMakeFiles/_fpgnu_swig.dir/depend

