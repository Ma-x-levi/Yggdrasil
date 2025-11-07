set(YGGDRASIL_PLATFORMS_ROOT "${CMAKE_CURRENT_LIST_DIR}")
set(YGGDRASIL_PLATFORMS_INCLUDE_ROOT "${YGGDRASIL_PLATFORMS_ROOT}/include")
set(YGGDRASIL_PLATFORMS_SOURCE_ROOT "${YGGDRASIL_PLATFORMS_ROOT}/src")
set(YGGDRASIL_PLATFORMS_THIRDPARTY_ROOT "${YGGDRASIL_PLATFORMS_ROOT}/3rdparty")

function(yggdrasil_collect_directories result base_dir)
    if(NOT IS_DIRECTORY "${base_dir}")
        set(${result} "" PARENT_SCOPE)
        return()
    endif()

    set(directories "${base_dir}")
    file(
        GLOB_RECURSE
        children
        LIST_DIRECTORIES TRUE
        "${base_dir}/*"
    )
    foreach(child IN LISTS children)
        if(IS_DIRECTORY "${child}")
            list(APPEND directories "${child}")
        endif()
    endforeach()
    list(REMOVE_DUPLICATES directories)
    set(${result} "${directories}" PARENT_SCOPE)
endfunction()

file(
    GLOB_RECURSE
    YGGDRASIL_PLATFORMS_SOURCES
    CONFIGURE_DEPENDS
    "${YGGDRASIL_PLATFORMS_SOURCE_ROOT}/*.c"
    "${YGGDRASIL_PLATFORMS_SOURCE_ROOT}/*.cc"
    "${YGGDRASIL_PLATFORMS_SOURCE_ROOT}/*.cxx"
    "${YGGDRASIL_PLATFORMS_SOURCE_ROOT}/*.cpp"
)

file(
    GLOB_RECURSE
    YGGDRASIL_PLATFORMS_HEADERS
    CONFIGURE_DEPENDS
    "${YGGDRASIL_PLATFORMS_INCLUDE_ROOT}/*.h"
    "${YGGDRASIL_PLATFORMS_INCLUDE_ROOT}/*.hh"
    "${YGGDRASIL_PLATFORMS_INCLUDE_ROOT}/*.hpp"
    "${YGGDRASIL_PLATFORMS_INCLUDE_ROOT}/*.hxx"
)

yggdrasil_collect_directories(
    YGGDRASIL_PLATFORMS_PUBLIC_INCLUDE_DIRS "${YGGDRASIL_PLATFORMS_INCLUDE_ROOT}"
)
yggdrasil_collect_directories(
    YGGDRASIL_PLATFORMS_PRIVATE_INCLUDE_DIRS "${YGGDRASIL_PLATFORMS_SOURCE_ROOT}"
)

if(IS_DIRECTORY "${YGGDRASIL_PLATFORMS_THIRDPARTY_ROOT}")
    yggdrasil_collect_directories(
        YGGDRASIL_PLATFORMS_THIRDPARTY_INCLUDE_DIRS
        "${YGGDRASIL_PLATFORMS_THIRDPARTY_ROOT}"
    )
    list(APPEND
        YGGDRASIL_PLATFORMS_PRIVATE_INCLUDE_DIRS
        ${YGGDRASIL_PLATFORMS_THIRDPARTY_INCLUDE_DIRS}
    )
endif()

list(REMOVE_DUPLICATES YGGDRASIL_PLATFORMS_SOURCES)
list(REMOVE_DUPLICATES YGGDRASIL_PLATFORMS_HEADERS)
list(REMOVE_DUPLICATES YGGDRASIL_PLATFORMS_PUBLIC_INCLUDE_DIRS)
list(REMOVE_DUPLICATES YGGDRASIL_PLATFORMS_PRIVATE_INCLUDE_DIRS)

add_library(yggdrasil_platforms ${YGGDRASIL_PLATFORMS_SOURCES})
add_library(yggdrasil::platforms ALIAS yggdrasil_platforms)

target_sources(
    yggdrasil_platforms
    PRIVATE
        ${YGGDRASIL_PLATFORMS_HEADERS}
)

target_include_directories(
    yggdrasil_platforms
    PUBLIC
        $<INSTALL_INTERFACE:include>
)

foreach(dir IN LISTS YGGDRASIL_PLATFORMS_PUBLIC_INCLUDE_DIRS)
    if(dir)
        target_include_directories(
            yggdrasil_platforms
            PUBLIC
                "$<BUILD_INTERFACE:${dir}>"
        )
    endif()
endforeach()

foreach(dir IN LISTS YGGDRASIL_PLATFORMS_PRIVATE_INCLUDE_DIRS)
    if(dir)
        target_include_directories(
            yggdrasil_platforms
            PRIVATE
                "$<BUILD_INTERFACE:${dir}>"
        )
    endif()
endforeach()

target_compile_features(yggdrasil_platforms PUBLIC cxx_std_20)

source_group(
    TREE "${YGGDRASIL_PLATFORMS_ROOT}"
    FILES
        ${YGGDRASIL_PLATFORMS_SOURCES}
        ${YGGDRASIL_PLATFORMS_HEADERS}
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(
    TARGETS yggdrasil_platforms
    EXPORT YggdrasilPlatformsTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    DIRECTORY "${YGGDRASIL_PLATFORMS_ROOT}/include/"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

set(
    YGGDRASIL_PLATFORMS_INSTALL_CONFIG_DIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/YggdrasilPlatforms"
)

configure_package_config_file(
    "${YGGDRASIL_PLATFORMS_ROOT}/cmake/YggdrasilPlatformsConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/YggdrasilPlatformsConfig.cmake"
    INSTALL_DESTINATION ${YGGDRASIL_PLATFORMS_INSTALL_CONFIG_DIR}
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/YggdrasilPlatformsConfig.cmake"
    DESTINATION ${YGGDRASIL_PLATFORMS_INSTALL_CONFIG_DIR}
)

install(
    EXPORT YggdrasilPlatformsTargets
    NAMESPACE yggdrasil::
    DESTINATION ${YGGDRASIL_PLATFORMS_INSTALL_CONFIG_DIR}
)
