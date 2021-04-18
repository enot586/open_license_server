cmake_minimum_required(VERSION 3.0)

###################### OPEN_LICENSE_SERVICE ######################

project(open_license_service VERSION 1.0.0 LANGUAGES CXX)

find_package(Boost 1.65 COMPONENTS system filesystem regex REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
include_directories( /usr/include )

message("boost include path:${Boost_INCLUDE_DIR}")
message("boost lib path:${Boost_LIBRARIES}")

set(SOURCE  
            ${PROJECT_SOURCE_DIR}/src/main.cpp
            ${PROJECT_SOURCE_DIR}/src/globals.h
            ${PROJECT_SOURCE_DIR}/src/json_parser.cpp
            ${PROJECT_SOURCE_DIR}/src/json_parser.h
            ${PROJECT_SOURCE_DIR}/src/communicator.cpp
            ${PROJECT_SOURCE_DIR}/src/communicator.h

            ${PROJECT_SOURCE_DIR}/src/protocols/protocol.h
            ${PROJECT_SOURCE_DIR}/src/protocols/json_protocol.h
            ${PROJECT_SOURCE_DIR}/src/protocols/json_protocol.cpp

            ${PROJECT_SOURCE_DIR}/src/commands/command.cpp
            ${PROJECT_SOURCE_DIR}/src/commands/command.h
            ${PROJECT_SOURCE_DIR}/src/commands/command_code.h
            ${PROJECT_SOURCE_DIR}/src/commands/command_factory.cpp
            ${PROJECT_SOURCE_DIR}/src/commands/command_factory.h

            ${PROJECT_SOURCE_DIR}/src/commands/cmd_get_version.hpp
            ${PROJECT_SOURCE_DIR}/src/commands/cmd_generate_license.hpp
)

add_executable(open_license_service ${SOURCE})

set_target_properties(open_license_service PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

TARGET_LINK_LIBRARIES( open_license_service LINK_PUBLIC ${Boost_LIBRARIES} )

###################### TEST_CLIENT ######################

project(test_client VERSION 1.0.0 LANGUAGES CXX)

set(TEST_CLIENT_SOURCE
            ${PROJECT_SOURCE_DIR}/src/test_client.cpp
)

add_executable(test_client ${TEST_CLIENT_SOURCE})

set_target_properties(test_client PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

TARGET_LINK_LIBRARIES( test_client LINK_PUBLIC ${Boost_LIBRARIES} )

###################### TEST_RUNNER ######################

project(test_runner VERSION 1.0.0 LANGUAGES CXX)

add_subdirectory(${PROJECT_SOURCE_DIR}/googletest)

message("libidr = ${PROJECT_BINARY_DIR}")
link_directories(${PROJECT_BINARY_DIR}/lib)

include_directories( ${PROJECT_SOURCE_DIR}/googletest/googletest/include)
include_directories( ${PROJECT_SOURCE_DIR}/googletest/googlemock/include)

set(TEST_RUNNER_SOURCE
            ${PROJECT_SOURCE_DIR}/src/json_parser.cpp
            ${PROJECT_SOURCE_DIR}/src/test/test_json_parser.cpp
            ${PROJECT_SOURCE_DIR}/src/test_runner.cpp
)

add_executable(test_runner ${TEST_RUNNER_SOURCE})

set_target_properties(test_runner PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

find_package(Threads REQUIRED)

TARGET_LINK_LIBRARIES( test_runner
            Threads::Threads
            ${PROJECT_BINARY_DIR}/lib/libgmock_maind.a
            ${PROJECT_BINARY_DIR}/lib/libgmockd.a
            ${PROJECT_BINARY_DIR}/lib/libgtestd.a
            ${PROJECT_BINARY_DIR}/lib/libgtest_maind.a
            boost_regex)
