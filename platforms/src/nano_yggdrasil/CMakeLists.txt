cmake_minimum_required(VERSION 3.10.0)
project(nano_yggdrasil)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(
    GLOB_RECURSE
    NANO_YGGDRASIL_PLATFORMS_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)


add_library(nano_yggdrasil STATIC ${NANO_YGGDRASIL_PLATFORMS_SOURCES})

target_include_directories(nano_yggdrasil PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
