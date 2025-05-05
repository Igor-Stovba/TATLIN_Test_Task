include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(tape_tests smoke_test.cpp)

target_link_libraries(tape_tests gtest_main)
target_link_libraries(tape_tests nlohmann_json::nlohmann_json)
target_link_libraries(tape_tests tape-lib)