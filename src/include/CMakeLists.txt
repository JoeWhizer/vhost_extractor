add_library(TBBTools TBBTools.cpp)

target_include_directories(TBBTools
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )