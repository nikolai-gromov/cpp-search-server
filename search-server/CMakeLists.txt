cmake_minimum_required(VERSION 3.16)
project(search_server)


set(CMAKE_CXX_STANDARD 17)


include_directories(.)


find_package(TBB REQUIRED)


add_executable(search_server
        concurrent_map.h
        document.cpp
        document.h
        log_duration.h
        main.cpp
        paginator.h
        process_queries.cpp
        process_queries.h
        read_input_functions.cpp
        read_input_functions.h
        request_queue.cpp
        request_queue.h
        search_server.cpp
        search_server.h
        string_processing.cpp
        string_processing.h
        test_example_functions.cpp
        test_example_functions.h)


target_link_libraries(${PROJECT_NAME} PUBLIC
        TBB::tbb
)