cmake_minimum_required(VERSION 3.17)
project(shell C)

set(CMAKE_C_STANDARD 99)

add_library(shell main.c mytop.h execommand.h main.h checkcommand.h getandanaOrder.h)