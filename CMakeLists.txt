if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

cmake_minimum_required(VERSION 2.8...3.20)
project(vhost_extractor VERSION 1.2)

find_package(Boost REQUIRED COMPONENTS filesystem system REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_subdirectory(src)
  install(TARGETS vhost_extractor DESTINATION bin)
endif()