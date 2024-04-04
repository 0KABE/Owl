cmake_minimum_required(VERSION 3.28)
project(Owl)

find_package(Boost 1.84.0 REQUIRED)
find_package(CLI11 1.9.1 REQUIRED)
find_package(spdlog 1.13.0 REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(OpenSSL 3.2.1 REQUIRED)
find_package(libsodium 1.0.18 REQUIRED)
find_package(uriparser 0.9.4 REQUIRED)
find_package(GTest 1.14.0 REQUIRED)

add_subdirectory(src)
add_subdirectory(test)
