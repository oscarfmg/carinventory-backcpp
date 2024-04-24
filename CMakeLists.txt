cmake_minimum_required(VERSION 3.10)

project(hcex)

include_directories(include)

add_executable(hcex "src/main.cpp" "src/model/Car.cpp")
# target_include_directories(hcex "${PROJECT_SOURCE_DIR}/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)