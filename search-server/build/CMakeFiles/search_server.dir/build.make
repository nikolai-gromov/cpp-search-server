# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nikolai/practicum/cpp-search-server/search-server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nikolai/practicum/cpp-search-server/search-server/build

# Include any dependencies generated for this target.
include CMakeFiles/search_server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/search_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/search_server.dir/flags.make

CMakeFiles/search_server.dir/document.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/document.cpp.o: ../document.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikolai/practicum/cpp-search-server/search-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/search_server.dir/document.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/search_server.dir/document.cpp.o -c /home/nikolai/practicum/cpp-search-server/search-server/document.cpp

CMakeFiles/search_server.dir/document.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/document.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nikolai/practicum/cpp-search-server/search-server/document.cpp > CMakeFiles/search_server.dir/document.cpp.i

CMakeFiles/search_server.dir/document.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/document.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nikolai/practicum/cpp-search-server/search-server/document.cpp -o CMakeFiles/search_server.dir/document.cpp.s

CMakeFiles/search_server.dir/main.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikolai/practicum/cpp-search-server/search-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/search_server.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/search_server.dir/main.cpp.o -c /home/nikolai/practicum/cpp-search-server/search-server/main.cpp

CMakeFiles/search_server.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nikolai/practicum/cpp-search-server/search-server/main.cpp > CMakeFiles/search_server.dir/main.cpp.i

CMakeFiles/search_server.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nikolai/practicum/cpp-search-server/search-server/main.cpp -o CMakeFiles/search_server.dir/main.cpp.s

CMakeFiles/search_server.dir/process_queries.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/process_queries.cpp.o: ../process_queries.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikolai/practicum/cpp-search-server/search-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/search_server.dir/process_queries.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/search_server.dir/process_queries.cpp.o -c /home/nikolai/practicum/cpp-search-server/search-server/process_queries.cpp

CMakeFiles/search_server.dir/process_queries.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/process_queries.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nikolai/practicum/cpp-search-server/search-server/process_queries.cpp > CMakeFiles/search_server.dir/process_queries.cpp.i

CMakeFiles/search_server.dir/process_queries.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/process_queries.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nikolai/practicum/cpp-search-server/search-server/process_queries.cpp -o CMakeFiles/search_server.dir/process_queries.cpp.s

CMakeFiles/search_server.dir/read_input_functions.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/read_input_functions.cpp.o: ../read_input_functions.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikolai/practicum/cpp-search-server/search-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/search_server.dir/read_input_functions.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/search_server.dir/read_input_functions.cpp.o -c /home/nikolai/practicum/cpp-search-server/search-server/read_input_functions.cpp

CMakeFiles/search_server.dir/read_input_functions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/read_input_functions.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nikolai/practicum/cpp-search-server/search-server/read_input_functions.cpp > CMakeFiles/search_server.dir/read_input_functions.cpp.i

CMakeFiles/search_server.dir/read_input_functions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/read_input_functions.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nikolai/practicum/cpp-search-server/search-server/read_input_functions.cpp -o CMakeFiles/search_server.dir/read_input_functions.cpp.s

CMakeFiles/search_server.dir/request_queue.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/request_queue.cpp.o: ../request_queue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikolai/practicum/cpp-search-server/search-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/search_server.dir/request_queue.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/search_server.dir/request_queue.cpp.o -c /home/nikolai/practicum/cpp-search-server/search-server/request_queue.cpp

CMakeFiles/search_server.dir/request_queue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/request_queue.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nikolai/practicum/cpp-search-server/search-server/request_queue.cpp > CMakeFiles/search_server.dir/request_queue.cpp.i

CMakeFiles/search_server.dir/request_queue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/request_queue.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nikolai/practicum/cpp-search-server/search-server/request_queue.cpp -o CMakeFiles/search_server.dir/request_queue.cpp.s

CMakeFiles/search_server.dir/search_server.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/search_server.cpp.o: ../search_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikolai/practicum/cpp-search-server/search-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/search_server.dir/search_server.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/search_server.dir/search_server.cpp.o -c /home/nikolai/practicum/cpp-search-server/search-server/search_server.cpp

CMakeFiles/search_server.dir/search_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/search_server.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nikolai/practicum/cpp-search-server/search-server/search_server.cpp > CMakeFiles/search_server.dir/search_server.cpp.i

CMakeFiles/search_server.dir/search_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/search_server.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nikolai/practicum/cpp-search-server/search-server/search_server.cpp -o CMakeFiles/search_server.dir/search_server.cpp.s

CMakeFiles/search_server.dir/string_processing.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/string_processing.cpp.o: ../string_processing.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikolai/practicum/cpp-search-server/search-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/search_server.dir/string_processing.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/search_server.dir/string_processing.cpp.o -c /home/nikolai/practicum/cpp-search-server/search-server/string_processing.cpp

CMakeFiles/search_server.dir/string_processing.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/string_processing.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nikolai/practicum/cpp-search-server/search-server/string_processing.cpp > CMakeFiles/search_server.dir/string_processing.cpp.i

CMakeFiles/search_server.dir/string_processing.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/string_processing.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nikolai/practicum/cpp-search-server/search-server/string_processing.cpp -o CMakeFiles/search_server.dir/string_processing.cpp.s

CMakeFiles/search_server.dir/test_example_functions.cpp.o: CMakeFiles/search_server.dir/flags.make
CMakeFiles/search_server.dir/test_example_functions.cpp.o: ../test_example_functions.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nikolai/practicum/cpp-search-server/search-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/search_server.dir/test_example_functions.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/search_server.dir/test_example_functions.cpp.o -c /home/nikolai/practicum/cpp-search-server/search-server/test_example_functions.cpp

CMakeFiles/search_server.dir/test_example_functions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/search_server.dir/test_example_functions.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nikolai/practicum/cpp-search-server/search-server/test_example_functions.cpp > CMakeFiles/search_server.dir/test_example_functions.cpp.i

CMakeFiles/search_server.dir/test_example_functions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/search_server.dir/test_example_functions.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nikolai/practicum/cpp-search-server/search-server/test_example_functions.cpp -o CMakeFiles/search_server.dir/test_example_functions.cpp.s

# Object files for target search_server
search_server_OBJECTS = \
"CMakeFiles/search_server.dir/document.cpp.o" \
"CMakeFiles/search_server.dir/main.cpp.o" \
"CMakeFiles/search_server.dir/process_queries.cpp.o" \
"CMakeFiles/search_server.dir/read_input_functions.cpp.o" \
"CMakeFiles/search_server.dir/request_queue.cpp.o" \
"CMakeFiles/search_server.dir/search_server.cpp.o" \
"CMakeFiles/search_server.dir/string_processing.cpp.o" \
"CMakeFiles/search_server.dir/test_example_functions.cpp.o"

# External object files for target search_server
search_server_EXTERNAL_OBJECTS =

search_server: CMakeFiles/search_server.dir/document.cpp.o
search_server: CMakeFiles/search_server.dir/main.cpp.o
search_server: CMakeFiles/search_server.dir/process_queries.cpp.o
search_server: CMakeFiles/search_server.dir/read_input_functions.cpp.o
search_server: CMakeFiles/search_server.dir/request_queue.cpp.o
search_server: CMakeFiles/search_server.dir/search_server.cpp.o
search_server: CMakeFiles/search_server.dir/string_processing.cpp.o
search_server: CMakeFiles/search_server.dir/test_example_functions.cpp.o
search_server: CMakeFiles/search_server.dir/build.make
search_server: /usr/lib/x86_64-linux-gnu/libtbb.so.2
search_server: CMakeFiles/search_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nikolai/practicum/cpp-search-server/search-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable search_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/search_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/search_server.dir/build: search_server

.PHONY : CMakeFiles/search_server.dir/build

CMakeFiles/search_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/search_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/search_server.dir/clean

CMakeFiles/search_server.dir/depend:
	cd /home/nikolai/practicum/cpp-search-server/search-server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nikolai/practicum/cpp-search-server/search-server /home/nikolai/practicum/cpp-search-server/search-server /home/nikolai/practicum/cpp-search-server/search-server/build /home/nikolai/practicum/cpp-search-server/search-server/build /home/nikolai/practicum/cpp-search-server/search-server/build/CMakeFiles/search_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/search_server.dir/depend

