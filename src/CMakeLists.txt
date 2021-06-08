add_subdirectory(include)
add_executable(vhost_extractor main.cpp include/TBBTools.cpp include/TBBConsole.cpp)
target_link_libraries(vhost_extractor Boost::system Boost::filesystem)