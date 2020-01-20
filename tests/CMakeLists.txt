cmake_minimum_required(VERSION 3.13)

project(openenclave-curl-tests C CXX)

include(CTest)

set(openenclave_curl_BUILD_DIR "" CACHE PATH "Path to openenclave-curl build folder")

find_package(openenclave REQUIRED CONFIG)

include(${openenclave_curl_BUILD_DIR}/targets.cmake)

add_subdirectory(enclave)
add_subdirectory(host)

add_test(NAME openenclave-curl-tests COMMAND tests.host $<TARGET_FILE:tests.enclave>)
