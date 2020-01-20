add_custom_command(OUTPUT tests_u.h tests_u.c tests_args.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../tests.edl
    COMMAND openenclave::oeedger8r --untrusted ${CMAKE_CURRENT_SOURCE_DIR}/../tests.edl)

add_executable(tests.host
    host.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/tests_u.c)

# Needed for the generated file tests_u.h
target_include_directories(tests.host PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(tests.host openenclave::oehostapp)
