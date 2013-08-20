cmake_minimum_required(VERSION 2.8)

project(chat)

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

file(GLOB_RECURSE SRCS src/*.cpp)

add_executable(chat ${SRCS})
# target_link_libraries(char ${libs})

