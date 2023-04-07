cmake_minimum_required(VERSION 3.16)
project(search_server)


set(CMAKE_CXX_STANDARD 17)


include_directories(.)


add_executable(search_server
        document.cpp
        document.h
        log_duration.h
        main.cpp
        paginator.h
        read_input_functions.cpp
        read_input_functions.h
        remove_duplicates.cpp
        remove_duplicates.h
        request_queue.cpp
        request_queue.h
        search_server.cpp
        search_server.h
        string_processing.cpp
        string_processing.h
        test_example_functions.cpp
        test_example_functions.h)