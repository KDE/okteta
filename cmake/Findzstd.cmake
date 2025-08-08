# SPDX-FileCopyrightText: 2025 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-2-Clause

#[=======================================================================[.rst:
Findzstd
--------

Try to find the zstd library.

This will define the following variables:

``zstd_FOUND``
    TRUE if (the requested version of) zstd is available
``zstd_VERSION``
    The version of zstd
``zstd_LIBRARIES``
    The libraries of zstd for use with target_link_libraries()
``zstd_INCLUDE_DIRS``
    The include dirs of zstd for use with target_include_directories()

If ``zstd_FOUND`` is TRUE, it will also define the following imported
target:

``zstd::libzstd``
    The zstd library

This custom find module is needed for now while not all distributions
provide CMake config files for zstd (as of zstd 1.5.7).
#]=======================================================================]

find_package(PkgConfig QUIET)
pkg_check_modules(PC_libzstd libzstd QUIET)

find_library(zstd_LIBRARIES
    NAMES zstd
    HINTS ${PC_libzstd_LIBRARY_DIRS}
)

find_path(zstd_INCLUDE_DIRS
    NAMES zstd.h
    HINTS ${PC_libzstd_INCLUDE_DIRS}
)

set(zstd_VERSION ${PC_libzstd_VERSION})
if(NOT zstd_VERSION)
    set(_header "${zstd_INCLUDE_DIRS}/zstd.h")
    if(EXISTS ${_header})
        file(STRINGS ${_header} _zstd_VERSION_MAJOR REGEX "^#define ZSTD_VERSION_MAJOR")
        string(REGEX MATCH "[0-9]+" zstd_VERSION_MAJOR ${_zstd_VERSION_MAJOR})
        file(STRINGS ${_header} _zstd_VERSION_MINOR REGEX "^#define ZSTD_VERSION_MINOR")
        string(REGEX MATCH "[0-9]+" zstd_VERSION_MINOR ${_zstd_VERSION_MINOR} )
        file(STRINGS ${_header} _zstd_VERSION_RELEASE REGEX "^#define ZSTD_VERSION_RELEASE")
        string(REGEX MATCH "[0-9]+" zstd_VERSION_RELEASE ${_zstd_VERSION_RELEASE} )
        set(zstd_VERSION "${zstd_VERSION_MAJOR}.${zstd_VERSION_MINOR}.${zstd_VERSION_RELEASE}")
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(zstd
    REQUIRED_VARS
        zstd_LIBRARIES
        zstd_INCLUDE_DIRS
    VERSION_VAR
        zstd_VERSION
)

if(zstd_FOUND AND NOT TARGET zstd::libzstd)
    add_library(zstd::libzstd UNKNOWN IMPORTED)
    set_target_properties(zstd::libzstd PROPERTIES
        IMPORTED_LOCATION "${zstd_LIBRARIES}"
        INTERFACE_COMPILE_OPTIONS "${PC_libzstd_CFLAGS}"
        INTERFACE_INCLUDE_DIRECTORIES "${zstd_INCLUDE_DIRS}"
    )
endif()

mark_as_advanced(zstd_LIBRARIES zstd_INCLUDE_DIRS zstd_VERSION)

include(FeatureSummary)
set_package_properties(zstd PROPERTIES
    DESCRIPTION "A fast lossless compression algorithm library"
    URL "https://github.com/facebook/zstd"
)
