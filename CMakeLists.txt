cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(hcex)

include_directories(include)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

set(CAR_SRCS 
"src/model/Car.cpp"
"src/model/CarMemRepository.cpp")

add_executable(hcex 
"src/main.cpp"
${CAR_SRCS})
# target_include_directories(hcex "${PROJECT_SOURCE_DIR}/")

enable_testing()
add_executable(carrepo_test "test/carrepo-test.cpp" ${CAR_SRCS})
target_link_libraries(carrepo_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(carrepo_test)