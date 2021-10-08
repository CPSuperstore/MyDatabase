cmake_minimum_required(VERSION 3.20)
project(MyDatabase C)

set(CMAKE_C_STANDARD 99)

add_executable(MyDatabase main.c utils/string_utils.h security/authentication.h disk/read_data.h utils/message_handler.h)
