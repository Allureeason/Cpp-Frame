cmake_minimum_required(VERSION 2.8)

project(busy)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

include(cmake/utils.cmake)

include_directories(.)
include_directories(/usr/local/include)


find_library(PTHREAD pthread)
find_library(YAMLCPP yaml-cpp)


set(LIB_SRC
    hxf/config.cc
    hxf/fiber.cc
    hxf/fd_manager.cc
    hxf/hook.cc
    hxf/iomanager.cc
    hxf/log.cc
    hxf/scheduler.cc
    hxf/thread.cc
    hxf/timer.cc
    hxf/util.cc
    )


add_library(hxf SHARED ${LIB_SRC})

force_redefine_file_macro_for_sources(hxf) # __FILE__
set(LIB_LIB
    hxf
    dl
    pthread
    yaml-cpp
    )


add_executable(test tests/test.cc)
add_dependencies(test hxf)
force_redefine_file_macro_for_sources(test)
target_link_libraries(test ${LIB_LIB})

add_executable(test_config tests/test_config.cc)
add_dependencies(test_config hxf)
force_redefine_file_macro_for_sources(test_config)
target_link_libraries(test_config ${LIB_LIB})

add_executable(test_thread tests/test_thread.cc)
add_dependencies(test_thread hxf)
force_redefine_file_macro_for_sources(test_thread)
target_link_libraries(test_thread ${LIB_LIB})

add_executable(test_util tests/test_util.cc)
add_dependencies(test_util hxf)
force_redefine_file_macro_for_sources(test_util)
target_link_libraries(test_util ${LIB_LIB})

add_executable(test_fiber tests/test_fiber.cc)
add_dependencies(test_fiber hxf)
force_redefine_file_macro_for_sources(test_fiber)
target_link_libraries(test_fiber ${LIB_LIB})

add_executable(test_scheduler tests/test_scheduler.cc)
add_dependencies(test_scheduler hxf)
force_redefine_file_macro_for_sources(test_scheduler)
target_link_libraries(test_scheduler ${LIB_LIB})

add_executable(test_iomanager tests/test_iomanager.cc)
add_dependencies(test_iomanager hxf)
force_redefine_file_macro_for_sources(test_iomanager)
target_link_libraries(test_iomanager ${LIB_LIB})

add_executable(test_hook tests/test_hook.cc)
add_dependencies(test_hook hxf)
force_redefine_file_macro_for_sources(test_hook)
target_link_libraries(test_hook ${LIB_LIB})


# add_executable(test_other tests/test_other.cc )
# add_dependencies(test_other hxf)
# force_redefine_file_macro_for_sources(test_other)
# target_link_libraries(test_other ${LIB_LIB})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

