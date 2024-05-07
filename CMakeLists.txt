cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(hcex LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

find_package( asio REQUIRED )

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG master
)
FetchContent_MakeAvailable(fmt)
  
include_directories(include "thirdparty/crow/" ${asio_INCLUDE})

set(CAR_SRCS 
"src/model/Car.cpp"
"src/model/CarMemRepository.cpp")

set(SRVRCROW_SRCS
"src/server_crow/ServerCrow.cpp"
)
add_executable(server-crow
${CAR_SRCS}
${SRVRCROW_SRCS}
"src/server_crow/main.cpp")
target_link_libraries(server-crow fmt::fmt)

enable_testing()
add_executable(carrepo_test "test/carrepo-test.cpp" ${CAR_SRCS})
target_link_libraries(carrepo_test GTest::gtest_main fmt::fmt)

add_executable(crowserver_test "test/servercrow-test.cpp" ${CAR_SRCS} ${SRVRCROW_SRCS})
target_link_libraries(crowserver_test GTest::gtest_main fmt::fmt)

include(GoogleTest)
gtest_discover_tests(carrepo_test "" AUTO)
gtest_discover_tests(crowserver_test "" AUTO)