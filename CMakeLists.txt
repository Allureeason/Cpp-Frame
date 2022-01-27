cmake_minimum_required(VERSION 2.8)

project(busy)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-functional -Wno-builtin-macro-redefined")

include(cmake/utils.cmake)

include_directories(.)
include_directories(/usr/local/include)


find_library(PTHREAD pthread)
find_library(YAMLCPP yaml-cpp)
# message("*****", ${YAMLCPP})



set(LIB_SRC
    hxf/log.cc
    hxf/util.cc
    hxf/config.cc
    hxf/thread.cc
    )


add_library(hxf SHARED ${LIB_SRC})

force_redefine_file_macro_for_sources(hxf) # __FILE__
set(LIB_LIB
    hxf
    ${PTHREAD}
    ${YAMLCPP}
    )


add_executable(test tests/test.cc)
add_dependencies(test hxf)
force_redefine_file_macro_for_sources(test)
target_link_libraries(test ${LIB_LIB})

add_executable(test_config tests/test_config.cc )
add_dependencies(test_config hxf)
force_redefine_file_macro_for_sources(test_config)
target_link_libraries(test_config ${LIB_LIB})

add_executable(test_thread tests/test_thread.cc )
add_dependencies(test_thread hxf)
force_redefine_file_macro_for_sources(test_thread)
target_link_libraries(test_thread ${LIB_LIB})

add_executable(test_util tests/test_util.cc )
add_dependencies(test_util hxf)
force_redefine_file_macro_for_sources(test_util)
target_link_libraries(test_util ${LIB_LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

