cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)

project(AWS_SDK_Examples)

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME}
                        aws-cpp-sdk-core
                        aws-cpp-sdk-dynamodb
                        aws-cpp-sdk-polly)
