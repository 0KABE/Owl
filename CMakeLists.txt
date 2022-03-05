cmake_minimum_required(VERSION 3.17)
project(Owl)

set(CMAKE_CXX_STANDARD 20)

message("Installing dependencies by Conan...")
execute_process(
        COMMAND bash -c "conan install \"${CMAKE_CURRENT_SOURCE_DIR}\" -if \"${CMAKE_CURRENT_SOURCE_DIR}/dependency\" --build=missing"
)

# Boost ASIO has some compatibility issue of C++20 Concept under clangd as language server
# But there no error during compiling even if without this definition
add_definitions(-DBOOST_ASIO_DISABLE_CONCEPTS)
# If required for backward compatibility, BOOST_ASIO_USE_TS_EXECUTOR_AS_DEFAULT can be defined,
# which changes the any_io_executor type alias to instead point to the executor polymorphic wrapper.
# https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/history.html#boost_asio.history.asio_1_18_0___boost_1_74
add_definitions(-DBOOST_ASIO_USE_TS_EXECUTOR_AS_DEFAULT)

# Since GCC 10 still not support C++20 Coroutine for production, Clang with libc++ is used under Linux platform
if(UNIX AND NOT APPLE)
    message("OS is detected as Linux, make sure the Clang and libc++ have been installed")
    add_compile_options(-stdlib=libc++)
    add_link_options(-lc++)
endif()

# Setup all dependencies
include(dependency/conanbuildinfo.cmake)
conan_basic_setup()

add_compile_options(-Wall)

add_subdirectory(src)
add_subdirectory(test)
