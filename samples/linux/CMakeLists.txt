cmake_minimum_required(VERSION 3.20)
project(linuxSample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SAMPLE_ROOT "${CMAKE_CURRENT_LIST_DIR}")
get_filename_component(YGGDRASIL_ROOT "${SAMPLE_ROOT}/../.." ABSOLUTE)
set(YGGDRASIL_PLATFORMS_ROOT "${YGGDRASIL_ROOT}/platforms")

add_subdirectory("${YGGDRASIL_PLATFORMS_ROOT}/src/nano_yggdrasil" ${CMAKE_BINARY_DIR}/nano_yggdrasil_build)

file(GLOB_RECURSE PROJECT_COMMON_SRC common/*.cpp)

message("--------------------" ${PROJECT_COMMON_SRC})


add_executable(${PROJECT_NAME}
    ${PROJECT_COMMON_SRC}
    main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE nano_yggdrasil)
