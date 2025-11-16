cmake_minimum_required(VERSION 3.20)
project(yggdrasil_backend_service_sample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SAMPLE_ROOT "${CMAKE_CURRENT_LIST_DIR}")
get_filename_component(YGGDRASIL_ROOT "${SAMPLE_ROOT}/../.." ABSOLUTE)
set(YGGDRASIL_PLATFORMS_ROOT "${YGGDRASIL_ROOT}/platforms")

add_subdirectory("${YGGDRASIL_PLATFORMS_ROOT}/src/nano_yggdrasil" ${CMAKE_BINARY_DIR}/nano_yggdrasil_build)

add_executable(
    yggdrasil_backend_service
    main.cpp
)

target_link_libraries(yggdrasil_backend_service PRIVATE nano_yggdrasil)
