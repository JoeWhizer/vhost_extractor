cmake_minimum_required(VERSION 3.1...3.20)
project(vhost_extractor VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)

install(TARGETS vhost_extractor DESTINATION bin)