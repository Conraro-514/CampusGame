cmake_minimum_required(VERSION 3.8)

# SET(CMAKE_C_COMPILER "E:/Qt/Tools/mingw810_64/bin/gcc.exe")
# SET(CMAKE_CXX_COMPILER "E:/Qt/Tools/mingw810_64/bin/g++.exe")
project(CampusGame_demo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb")
SET(PROJECT_ROOT_PATH "${PROJECT_SOURCE_DIR}")
SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}")
SET(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib/")
link_directories(${PROJECT_BINARY_DIR}/lib/)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXE_LINKER_FLAGS "-pthread")

# 在此处设置自己的路径
set(OpenCV_DIR "E:/environments/mingw/OpenCV")
set(BOOST_ROOT "E:/environments/mingw/boost_1_80_0")
set(CampusGame_DIR "E:/environments/mingw/CampusGame/lib/cmake/CampusGame")
set(jsoncpp_DIR "E:/environments/mingw/jsoncpp/lib/cmake/jsoncpp")

# ################### OpenCV
find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})

# ################### BOOST
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# ################### JsonCPP
find_package(jsoncpp REQUIRED)

# ################### CampusGame
find_package(CampusGame REQUIRED)
include_directories(${JSON_INC_PATH})

FILE(GLOB INCLUDE_FILES "include/*.h")
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(./src PROJECT_SRCS)
# ################### My Files
add_subdirectory(GetContours)
add_subdirectory(ColorDetcetion)









include_directories(./A)
include_directories(./ColorDetcetion)








add_executable(CampusGame_demo "main.cpp" ${PROJECT_SRCS})
target_link_libraries(CampusGame_demo ${OpenCV_LIBS} CampusGame)