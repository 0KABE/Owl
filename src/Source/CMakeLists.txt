cmake_minimum_required(VERSION 3.28)
project(Source)

file(GLOB_RECURSE SRCS *.cc)

add_library(${PROJECT_NAME} OBJECT ${SRCS})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        boost::boost
        CLI11::CLI11
        spdlog::spdlog
        rapidjson
        openssl::openssl
        libsodium::libsodium
        uriparser::uriparser
)
