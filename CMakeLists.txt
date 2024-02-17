cmake_minimum_required(VERSION 3.25)
project(project_1)

# Set the C++ Standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Finding the required packages
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

# Define the library built from src/mros_json.cpp
add_library(json_parser_lib src/mros_json.cpp)
# Include directories for json_parser_lib if needed
target_include_directories(json_parser_lib PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Define the library for the sockets
add_library(socket_lib 
    src/socket/client_socket.cpp
    src/socket/connection_socket.cpp
    src/socket/server_socket.cpp
    src/socket/socket.cpp
 
    src/socket/message_socket/client_message_socket.cpp
    src/socket/message_socket/connection_message_socket.cpp
    src/socket/message_socket/message_socket.cpp
)

# Include directories for socket_lib if needed
target_include_directories(socket_lib PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)


# Executables for controller and system
add_executable(controller src/controller.cpp)
add_executable(system src/system.cpp)

# Linking the executables with the previously defined libraries
target_link_libraries(controller 
    json_parser_lib
    socket_lib
    Threads::Threads
)

target_link_libraries(system 
    json_parser_lib
    socket_lib
    Threads::Threads
)

# Enable testing with CTest
enable_testing()

# Add test executable target for JSON
add_executable(test_mros_json test/test_mros_json.cpp)
# Link the test executable with GTest, the JSON library, and threads
target_link_libraries(test_mros_json
    GTest::GTest 
    GTest::Main 
    json_parser_lib
    Threads::Threads
)

add_executable(test_twist test/test_twist.cpp)
add_executable(test_pose test/test_pose.cpp)
add_executable(test_message_socket test/test_message_socket.cpp)

target_link_libraries(test_twist
    GTest::GTest 
    GTest::Main 
    socket_lib 
    json_parser_lib 
    Threads::Threads
)

target_link_libraries(test_pose
    GTest::GTest 
    GTest::Main 
    socket_lib 
    json_parser_lib 
    Threads::Threads
)

target_link_libraries(test_message_socket
    GTest::GTest 
    GTest::Main 
    socket_lib 
    Threads::Threads
)

# Add the GTest-based test so it can be run with CTest
add_test(NAME test_twist COMMAND test_twist)
add_test(NAME test_pose COMMAND test_pose)
add_test(NAME test_mros_json COMMAND test_mros_json)
add_test(NAME test_message_socket COMMAND test_message_socket)