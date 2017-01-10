cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)

project(AWS_SDK_Test_)

# Locate the AWS C++ SDK
find_package(aws-cpp-sdk-core)
find_package(aws-cpp-sdk-dynamodb required)
find_package(aws-cpp-sdk-polly required)

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME}
                        aws-cpp-sdk-dynamodb
                        aws-cpp-sdk-polly)
