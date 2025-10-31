set(YGGDRASIL_PLATFORMS_ROOT "${CMAKE_CURRENT_LIST_DIR}")

file(
    GLOB_RECURSE
    YGGDRASIL_PLATFORMS_SOURCES
    CONFIGURE_DEPENDS
    "${YGGDRASIL_PLATFORMS_ROOT}/src/*.c"
    "${YGGDRASIL_PLATFORMS_ROOT}/src/*.cc"
    "${YGGDRASIL_PLATFORMS_ROOT}/src/*.cxx"
    "${YGGDRASIL_PLATFORMS_ROOT}/src/*.cpp"
)

file(
    GLOB_RECURSE
    YGGDRASIL_PLATFORMS_HEADERS
    CONFIGURE_DEPENDS
    "${YGGDRASIL_PLATFORMS_ROOT}/include/*.h"
    "${YGGDRASIL_PLATFORMS_ROOT}/include/*.hh"
    "${YGGDRASIL_PLATFORMS_ROOT}/include/*.hpp"
    "${YGGDRASIL_PLATFORMS_ROOT}/include/*.hxx"
)

list(REMOVE_DUPLICATES YGGDRASIL_PLATFORMS_SOURCES)
list(REMOVE_DUPLICATES YGGDRASIL_PLATFORMS_HEADERS)

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
        $<BUILD_INTERFACE:${YGGDRASIL_PLATFORMS_ROOT}/include>
        $<INSTALL_INTERFACE:include>
)

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
