cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(carinv LANGUAGES CXX)

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

FetchContent_Declare(
  libpqxx
  GIT_REPOSITORY https://github.com/jtv/libpqxx.git
  GIT_TAG 7.9.2
)
FetchContent_MakeAvailable(libpqxx)

set(libpqxx_INCLUDE
"build/_deps/libpqxx-src/include"
"build/_deps/libpqxx-build/include")

include_directories(include "thirdparty/crow/" ${asio_INCLUDE} ${libpqxx_INCLUDE})

set(CAR_SRCS 
"src/model/Car.cpp"
"src/model/CarMemRepository.cpp"
"src/model/CarPgSqlRepository.cpp")

set(SRVRCROW_SRCS
"src/server_crow/ServerCrow.cpp"
)
add_executable(server-crow
${CAR_SRCS}
${SRVRCROW_SRCS}
"src/server_crow/main.cpp")
target_compile_definitions(server-crow PUBLIC USE_PGSQL_REPO=1)
target_link_libraries(server-crow pqxx fmt::fmt)

enable_testing()
add_executable(carrepo_test "test/carrepo-test.cpp" ${CAR_SRCS})
target_link_libraries(carrepo_test pqxx GTest::gtest_main fmt::fmt)

add_executable(carpg_test "test/carpg-test.cpp" ${CAR_SRCS})
target_link_libraries(carpg_test pqxx GTest::gtest_main fmt::fmt)

add_executable(crowserver_test "test/servercrow-test.cpp" ${CAR_SRCS} ${SRVRCROW_SRCS})
target_link_libraries(crowserver_test pqxx GTest::gtest_main fmt::fmt)

include(GoogleTest)
gtest_discover_tests(carrepo_test "" AUTO)
gtest_discover_tests(carpg_test "" AUTO)
gtest_discover_tests(crowserver_test "" AUTO)