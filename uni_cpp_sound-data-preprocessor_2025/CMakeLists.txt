# CMakeList.txt
cmake_minimum_required (VERSION 3.8)

include(FetchContent)

project ("uni_cpp_sound-data-preprocessor_2025")

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable (uni_cpp_sound-data-preprocessor_2025 "uni_cpp_sound-data-preprocessor_2025.cpp")

# Fetch csv-parser and make it available to my-executable
FetchContent_Declare(
  csv  
  GIT_REPOSITORY https://github.com/vincentlaucsb/csv-parser.git
  GIT_SHALLOW TRUE 
  GIT_TAG 2.2.1
) 

FetchContent_MakeAvailable(csv)

target_include_directories(uni_cpp_sound-data-preprocessor_2025 PRIVATE csv)
target_link_libraries(uni_cpp_sound-data-preprocessor_2025 PRIVATE csv)

# Only add gcov linking when using a compiler that supports/needs it
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_libraries(uni_cpp_sound-data-preprocessor_2025 PRIVATE gcov)
endif()