cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(hcex)

include_directories(include)

add_executable(hcex 
"src/main.cpp"
"src/model/Car.cpp"
"src/model/CarMemRepository.cpp")
# target_include_directories(hcex "${PROJECT_SOURCE_DIR}/")
