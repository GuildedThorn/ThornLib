cmake_minimum_required(VERSION 3.21)

project(ThornLib
    VERSION 0.1.0
    DESCRIPTION "Portable C99 library template"
    HOMEPAGE_URL "https://github.com/GuildedThorn/ThornLib"
    LANGUAGES C
)

# ---------------------------------------------------------------------------
# Configuration
#
# When adapting this template, the values below are the only ones that need
# to change for a normal rename.
# ---------------------------------------------------------------------------
set(THORN_LIBRARY_TARGET thorn)
set(THORN_NAMESPACE Thorn)

include(GNUInstallDirs)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(ThornWarnings)
include(ThornSanitizers)

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(THORN_BUILD_TESTS "Build the unit tests" ${PROJECT_IS_TOP_LEVEL})
option(THORN_BUILD_EXAMPLES "Build the examples" ${PROJECT_IS_TOP_LEVEL})
option(THORN_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(THORN_ENABLE_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(THORN_INSTALL "Generate install and CMake package rules" ${PROJECT_IS_TOP_LEVEL})
option(THORN_INSTALL_PKGCONFIG "Install a pkg-config file" ON)

# ---------------------------------------------------------------------------
# Generated headers
# ---------------------------------------------------------------------------
set(THORN_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/thorn/core/version.h.in"
    "${THORN_GENERATED_INCLUDE_DIR}/thorn/core/version.h"
    @ONLY
)

# ---------------------------------------------------------------------------
# Library
#
# The library flavour is controlled by the standard BUILD_SHARED_LIBS cache
# variable:
#
#   cmake -S . -B build -DBUILD_SHARED_LIBS=ON    # shared
#   cmake -S . -B build -DBUILD_SHARED_LIBS=OFF   # static (default)
# ---------------------------------------------------------------------------
set(THORN_SOURCES
    src/core/version.c
)

add_library(${THORN_LIBRARY_TARGET} ${THORN_SOURCES})
add_library(${THORN_NAMESPACE}::${THORN_LIBRARY_TARGET} ALIAS ${THORN_LIBRARY_TARGET})

target_compile_features(${THORN_LIBRARY_TARGET} PUBLIC c_std_99)

set_target_properties(${THORN_LIBRARY_TARGET} PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME ${THORN_LIBRARY_TARGET}
)

target_include_directories(${THORN_LIBRARY_TARGET}
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${THORN_GENERATED_INCLUDE_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(${THORN_LIBRARY_TARGET} PRIVATE THORN_BUILDING_LIBRARY)
else()
    target_compile_definitions(${THORN_LIBRARY_TARGET} PUBLIC THORN_STATIC_DEFINE)
endif()

thorn_set_warnings(${THORN_LIBRARY_TARGET})
if(THORN_ENABLE_SANITIZERS)
    thorn_enable_sanitizers(${THORN_LIBRARY_TARGET})
endif()

# ---------------------------------------------------------------------------
# Tests and examples
# ---------------------------------------------------------------------------
if(THORN_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(THORN_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ---------------------------------------------------------------------------
# Installation
# ---------------------------------------------------------------------------
if(THORN_INSTALL)
    include(CMakePackageConfigHelpers)

    install(TARGETS ${THORN_LIBRARY_TARGET}
        EXPORT ${PROJECT_NAME}Targets
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )

    install(DIRECTORY "${THORN_GENERATED_INCLUDE_DIR}/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING PATTERN "*.h"
    )

    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
        FILES_MATCHING PATTERN "*.h"
    )

    set(THORN_CMAKE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION "${THORN_CMAKE_INSTALL_DIR}"
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "${THORN_CMAKE_INSTALL_DIR}"
    )

    install(EXPORT ${PROJECT_NAME}Targets
        NAMESPACE ${THORN_NAMESPACE}::
        DESTINATION "${THORN_CMAKE_INSTALL_DIR}"
    )

    if(THORN_INSTALL_PKGCONFIG)
        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/thorn.pc.in"
            "${CMAKE_CURRENT_BINARY_DIR}/thorn.pc"
            @ONLY
        )

        install(FILES "${CMAKE_CURRENT_BINARY_DIR}/thorn.pc"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
        )
    endif()
endif()
