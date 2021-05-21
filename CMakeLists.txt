cmake_minimum_required(VERSION 3.16)
project(FinalProjectCPP)

set(CMAKE_CXX_STANDARD 14)

#include_directories(/usr/local/Cellar/boost/1.72.0_2/include/boost)

include_directories(/usr/local/Cellar/boost/1.72.0_2/include)
add_subdirectory(Tests)

add_executable(FinalProjectCPP main.cpp Model.cpp Menu.cpp Menu.h BaseTable.cpp BaseTable.h View.cpp View.h)