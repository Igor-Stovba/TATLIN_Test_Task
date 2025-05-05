cmake_minimum_required(VERSION 3.11)
project(bin-tape LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# dependance for json handling
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2  
)
FetchContent_MakeAvailable(nlohmann_json)

add_executable(${PROJECT_NAME} src/main.cpp
                        src/fileTape.cpp
                        src/sorterTape.cpp)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)

add_library(tape-lib STATIC src/main.cpp
                            src/fileTape.cpp
                            src/sorterTape.cpp)
target_compile_options(tape-lib PRIVATE -DDEBU) # only for debg, 
target_link_libraries(tape-lib nlohmann_json::nlohmann_json)

#for tests
add_subdirectory(tests)

add_dependencies(tape_tests tape-lib)
