project(Owl_UnitTest)

file(GLOB_RECURSE SRCS *.cc)

include_directories(../src/Include)

add_executable(${PROJECT_NAME}
        main.cc
        ${SRCS}
        )

target_link_libraries(
        ${PROJECT_NAME}
        Source
        GTest::GTest
)

include(GoogleTest)
enable_testing()
gtest_discover_tests(${PROJECT_NAME})
