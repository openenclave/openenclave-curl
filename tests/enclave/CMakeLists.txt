add_custom_command(OUTPUT tests_t.h tests_t.c tests_args.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../tests.edl
    COMMAND openenclave::oeedger8r --trusted ${CMAKE_CURRENT_SOURCE_DIR}/../tests.edl)

add_executable(tests.enclave
    enclave.cpp
    curl_helper.cpp
    curl_helper.h
    ca_certificates.h
    ${CMAKE_CURRENT_BINARY_DIR}/tests_t.c)

# Needed for the generated file tests_t.h
target_include_directories(tests.enclave PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(tests.enclave
    openenclave-curl::libcurl
    openenclave::oeenclave
    openenclave::oelibc)
