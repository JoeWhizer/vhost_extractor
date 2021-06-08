add_subdirectory(include)
add_executable(vhost_extractor main.cpp include/TBBTools.cpp include/TBBConsole.cpp include/TMethods.hpp)
target_link_libraries(vhost_extractor Boost::system Boost::filesystem)