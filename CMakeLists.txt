cmake_minimum_required(VERSION 3.10.2)
project(f1telem)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ---------------------------------
# LIBS
# libf1telem
file(GLOB LIBF1TELEM_SRC "${CMAKE_BINARY_DIR}/src/f1telem/*.cpp")
add_library(libf1telem STATIC ${LIBF1TELEM_SRC})
set_target_properties(libf1telem PROPERTIES LINKER_LANGUAGE CXX)
# ---------------------------------

# ---------------------------------
# EXECUTABLES
# sealvm
file(GLOB TESTAPP_SRC "${CMAKE_BINARY_DIR}/cmd/testapp.cpp")
add_executable(testapp ${TESTAPP_SRC})
target_link_libraries(testapp libf1telem)
# ---------------------------------

include_directories(
    "${CMAKE_BINARY_DIR}/include"
)