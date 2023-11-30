cmake_minimum_required(VERSION 3.24)
project(DesEncrypt C)

set(CMAKE_C_STANDARD 11)

add_executable(DesEncrypt crypt.c SwitchTable.h TextSwitch.h Des.h OpMode.h)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
