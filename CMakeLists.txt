cmake_minimum_required (VERSION 2.6)
project (human_readable)

include_directories("${PROJECT_SOURCE_DIR}")

add_executable(hr main.c)

add_library(hrlib hr.c)

target_link_libraries (hr hrlib)

include(CTest)
add_test (hrhelp hr -h)
