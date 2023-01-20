cmake_minimum_required(VERSION 3.22)
project(GOL)

set(CMAKE_CXX_STANDARD 14)

add_executable(GOL main.cpp Rule.cpp Rule.h Field.cpp Field.h Parser.cpp Parser.h Preset.h)
