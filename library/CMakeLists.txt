cmake_minimum_required(VERSION 3.13)

project(openenclave-curl C CXX)

get_directory_property(has_parent PARENT_DIRECTORY)
if (has_parent)
    message(FATAL_ERROR "${PROJECT_NAME} cannot be included with add_subdirectory() but needs to be built stand-alone")
endif()

cmake_policy(SET CMP0079 NEW)

find_package(openenclave REQUIRED CONFIG)

# libcurl for use in enclaves
set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(HTTP_ONLY ON CACHE BOOL "" FORCE)
set(HAVE_LINUX_TCP_H OFF CACHE BOOL "" FORCE)
set(CURL_DISABLE_COOKIES ON CACHE BOOL "" FORCE)
set(CURL_ZLIB OFF CACHE BOOL "" FORCE)
set(ENABLE_UNIX_SOCKETS OFF CACHE BOOL "" FORCE)
# CURL_CA_BUNDLE_SET & CURL_CA_PATH_SET are not reset correctly in incremental builds.
# (bug in libcurl's CMake scripts)
set(CURL_CA_BUNDLE_SET FALSE CACHE BOOL "" FORCE)
set(CURL_CA_PATH_SET FALSE CACHE BOOL "" FORCE)
if (CMAKE_BUILD_TYPE STREQUAL Debug)
  set(ENABLE_DEBUG ON CACHE BOOL "" FORCE)
endif()

set(CMAKE_USE_LIBSSH2 OFF CACHE BOOL "" FORCE)
set(CMAKE_USE_MBEDTLS ON CACHE BOOL "" FORCE)
# Satisfy curl's FindMbedTLS module, mbedtls supplied by oeenclave.
set(MBEDTLS_LIBRARY "" CACHE STRING "" FORCE)
set(MBEDX509_LIBRARY "" CACHE STRING "" FORCE)
set(MBEDCRYPTO_LIBRARY "" CACHE STRING "" FORCE)
set(MBEDTLS_INCLUDE_DIRS "" CACHE STRING "" FORCE)
# Avoid that curl links to dl.
set(HAVE_LIBDL OFF CACHE BOOL "" FORCE)
set(HAVE_CLOCK_GETTIME_MONOTONIC OFF CACHE BOOL "" FORCE)
set(HAVE_ALARM OFF CACHE BOOL "" FORCE)
set(ENABLE_THREADED_RESOLVER OFF CACHE BOOL "" FORCE)
set(HAVE_GETNAMEINFO OFF CACHE BOOL "" FORCE)
add_compile_definitions(CURL_DISABLE_NETRC USE_BLOCKING_SOCKETS)

add_subdirectory(curl EXCLUDE_FROM_ALL)

add_custom_target(dummy ALL DEPENDS libcurl)

target_link_libraries(libcurl
    openenclave::oeenclave
    openenclave::oecryptombedtls
    openenclave::oelibcxx
    openenclave::oehostsock
    openenclave::oehostresolver
    )

target_include_directories(libcurl PUBLIC ${OE_INCLUDEDIR}/openenclave/3rdparty)

target_sources(libcurl PRIVATE stubs_undefined.c)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/curl/include DESTINATION include/oecurl)
install(TARGETS libcurl EXPORT oecurl-config
        ARCHIVE DESTINATION lib/oecurl 
        INCLUDES DESTINATION include/oecurl)

install(EXPORT oecurl-config 
        DESTINATION lib/oecurl
        NAMESPACE ${PROJECT_NAME}::
        EXPORT_LINK_INTERFACE_LIBRARIES)

export(
    TARGETS libcurl
    NAMESPACE ${PROJECT_NAME}::
    FILE ${CMAKE_BINARY_DIR}/targets.cmake)