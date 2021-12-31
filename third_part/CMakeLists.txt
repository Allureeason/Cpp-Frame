cmake_minimum_required(VERSION 2.8)

project(busy)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O0 -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-functional")

include_directories(.)
include_directories(/usr/local/include)



find_library(YAMLCPP yaml-cpp)
message("*****", ${YAMLCPP})

set(LIB_SRC
    hxf/log.cc
    hxf/util.cc
    hxf/config.cc
    )


add_library(hxf SHARED ${LIB_SRC})


add_executable(test tests/test.cc)
add_dependencies(test hxf)
target_link_libraries(test hxf ${YAMLCPP})
#
add_executable(test_config tests/test_config.cc )
add_dependencies(test_config hxf yaml-cpp)
target_link_libraries(test_config 
                    hxf
                    ${YAMLCPP}

                    )

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

