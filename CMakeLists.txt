cmake_minimum_required(VERSION 3.30)
project(calculator C)

set(CMAKE_C_STANDARD 11)

add_custom_target(
        run ALL
        COMMAND make
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

