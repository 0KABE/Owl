cmake_minimum_required(VERSION 3.28)
project(Owl)

include_directories(Include)

add_subdirectory(Source)

add_executable(${PROJECT_NAME} main.cc)

target_link_libraries(
        ${PROJECT_NAME}
        Source
)
