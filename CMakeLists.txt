cmake_minimum_required(VERSION 3.13)

project(superbuild C CXX)

include(ExternalProject)
include(CTest)

set(COMMON_CMAKE_CACHE_ARGS
    -Dopenenclave_DIR:STRING=${openenclave_DIR}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_C_COMPILER:STRING=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
)

ExternalProject_Add(openenclave-curl
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/library
    CMAKE_CACHE_ARGS ${COMMON_CMAKE_CACHE_ARGS}
    INSTALL_COMMAND ""
    BUILD_ALWAYS ON
    )

ExternalProject_Get_property(openenclave-curl BINARY_DIR)
set(openenclave_curl_BUILD_DIR ${BINARY_DIR})

ExternalProject_Add(tests
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/tests
    CMAKE_CACHE_ARGS
        ${COMMON_CMAKE_CACHE_ARGS}
        -Dopenenclave_curl_BUILD_DIR:STRING=${openenclave_curl_BUILD_DIR}
    BUILD_ALWAYS ON
    TEST_AFTER_INSTALL ON
    INSTALL_COMMAND ""
    DEPENDS openenclave-curl
    )
