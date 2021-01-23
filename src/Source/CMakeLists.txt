cmake_minimum_required(VERSION 3.0.0)
project(Source)

file(GLOB_RECURSE SRCS *.cc)

add_library(${PROJECT_NAME} OBJECT ${SRCS})