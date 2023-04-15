#
# Okteta Private Macros
#
# SPDX-FileCopyrightText: 2018, 2019 Friedrich W. H. Kossebau <kossebau@kde.org>
#
# SPDX-License-Identifier: BSD-3-Clause
#

# ECM
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)
include(ECMGeneratePkgConfigFile)
# CMake
include(CMakePackageConfigHelpers)
include(CMakeParseArguments)
include(CheckCXXCompilerFlag)

# helper macros
function(_okteta_target_name _varName _baseName)
    string(CONCAT _namespacePrefix ${ARGN})
    set(${_varName} "${_namespacePrefix}${_baseName}" PARENT_SCOPE)
endfunction()


function(_okteta_setup_namespace)
    set(options
    )
    set(oneValueArgs
        BASE_NAME
        NAMESPACEPREFIX_VAR
        VERSIONEDNAMESPACEPREFIX_VAR
        VERSIONEDNAMESPACE_VAR
    )
    set(multiValueArgs
        NAMESPACE
        ABIVERSION
    )
    cmake_parse_arguments(OKTETA_SETUP_NAMESPACE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    string(CONCAT _namespacePrefix ${OKTETA_SETUP_NAMESPACE_NAMESPACE})

    if (OKTETA_SETUP_NAMESPACE_ABIVERSION)
        list(LENGTH OKTETA_SETUP_NAMESPACE_ABIVERSION _abiversionCount)
        list(LENGTH OKTETA_SETUP_NAMESPACE_NAMESPACE _namespaceCount)
        if (NOT _abiversionCount EQUAL _namespaceCount)
            message(FATAL_ERROR "ABIVERSION needs to have as many elements as NAMESPACE if used")
        endif()
        math(EXPR _namespaceLastIndex "${_namespaceCount} - 1")

        set(_versioned_namespace)
        foreach(val RANGE ${_namespaceLastIndex})
            list(GET OKTETA_SETUP_NAMESPACE_NAMESPACE ${val} _namespace)
            list(GET OKTETA_SETUP_NAMESPACE_ABIVERSION ${val} _abiversion)
            list(APPEND _versioned_namespace "${_namespace}${_abiversion}")
        endforeach()
        string(CONCAT _versionedNamespacePrefix ${_versioned_namespace})
    else()
        set(_versioned_namespace ${OKTETA_SETUP_NAMESPACE_NAMESPACE})
        set(_versionedNamespacePrefix "${_namespacePrefix}")
    endif()

    if (OKTETA_SETUP_NAMESPACE_NAMESPACEPREFIX_VAR)
        set(${OKTETA_SETUP_NAMESPACE_NAMESPACEPREFIX_VAR} ${_namespacePrefix} PARENT_SCOPE)
    endif()
    if (OKTETA_SETUP_NAMESPACE_VERSIONEDNAMESPACEPREFIX_VAR)
        set(${OKTETA_SETUP_NAMESPACE_VERSIONEDNAMESPACEPREFIX_VAR} ${_versionedNamespacePrefix} PARENT_SCOPE)
    endif()
    if (OKTETA_SETUP_NAMESPACE_VERSIONEDNAMESPACE_VAR)
        set(${OKTETA_SETUP_NAMESPACE_VERSIONEDNAMESPACE_VAR} ${_versioned_namespace} PARENT_SCOPE)
    endif()
endfunction()

function(_okteta_generate_version_code)
    set(options
    )
    set(oneValueArgs
        CODE_VARIABLE
        PREFIX
        VERSION
    )
    set(multiValueArgs
    )
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Omit any leading "0" in extraction, would otherwise yield an octal value string in C++ (08, 09)
    string(REGEX REPLACE "^0*([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" _version_major "${ARGS_VERSION}")
    string(REGEX REPLACE "^[0-9]+\\.0*([0-9]+)\\.[0-9]+.*" "\\1" _version_minor "${ARGS_VERSION}")
    string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.0*([0-9]+).*" "\\1" _version_patch "${ARGS_VERSION}")
    math(EXPR _version_hexnumber "${_version_major}*65536 + ${_version_minor}*256 + ${_version_patch}" OUTPUT_FORMAT HEXADECIMAL)

    set(_content
"#define ${ARGS_PREFIX}_VERSION_STRING \"${ARGS_VERSION}\"
#define ${ARGS_PREFIX}_VERSION_MAJOR ${_version_major}
#define ${ARGS_PREFIX}_VERSION_MINOR ${_version_minor}
#define ${ARGS_PREFIX}_VERSION_PATCH ${_version_patch}
#define ${ARGS_PREFIX}_VERSION ${_version_hexnumber}
"
    )
    set(${ARGS_CODE_VARIABLE} "${_content}" PARENT_SCOPE)
endfunction()

function(_okteta_generate_export_code)
    set(options
    )
    set(oneValueArgs
        CODE_VARIABLE
        PREFIX
        TARGET_NAME
    )
    set(multiValueArgs
    )
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT WIN32)
        check_cxx_compiler_flag("-fvisibility=hidden" COMPILER_SUPPORTS_HIDDEN_VISIBILITY)
    endif()

    set(_export_macro_name "${ARGS_PREFIX}_EXPORT")
    set(_content "#ifndef ${_export_macro_name}\n")
    if(WIN32)
        get_target_property(_building_lib_flag ${ARGS_TARGET_NAME} DEFINE_SYMBOL)

        if(NOT _building_lib_flag)
            # calculate cmake default preprocessor definition set when building a shared library
            set(_building_lib_flag ${ARGS_TARGET_NAME}_EXPORTS)
            string(MAKE_C_IDENTIFIER ${_building_lib_flag} _building_lib_flag)
        endif()

        string(APPEND _content
"#  ifdef ${_building_lib_flag}
     // Library is built
#    define ${_export_macro_name} __declspec(dllexport)
#  else
     // Library is consumed
#    define ${_export_macro_name} __declspec(dllimport)
#  endif
"
        )
    else()
        if(COMPILER_SUPPORTS_HIDDEN_VISIBILITY)
            string(APPEND _content "#  define ${_export_macro_name} __attribute__((visibility(\"default\")))\n")
        else()
            string(APPEND _content "#  define ${_export_macro_name}\n")
        endif()
    endif()
    string(APPEND _content "#endif\n")

    set(_no_export_macro_name "${ARGS_PREFIX}_NO_EXPORT")
    string(APPEND _content "#ifndef ${_no_export_macro_name}\n")
    if(WIN32)
        string(APPEND _content "#  define ${_no_export_macro_name}\n")
    else()
        if(COMPILER_SUPPORTS_HIDDEN_VISIBILITY)
            string(APPEND _content "#  define ${_no_export_macro_name} __attribute__((visibility(\"hidden\")))\n")
        else()
            string(APPEND _content "#  define ${_no_export_macro_name}\n")
        endif()
    endif()
    string(APPEND _content "#endif\n")

    set(${ARGS_CODE_VARIABLE} "${_content}" PARENT_SCOPE)
endfunction()

function(_okteta_generate_header_file)
    set(options
    )
    set(oneValueArgs
        CODE
        INCLUDE_GUARD_NAME
        EXPORT_FILE_NAME
    )
    set(multiValueArgs
    )
    cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    file(GENERATE OUTPUT ${ARGS_EXPORT_FILE_NAME} CONTENT
"#ifndef ${ARGS_INCLUDE_GUARD_NAME}
#define ${ARGS_INCLUDE_GUARD_NAME}

${ARGS_CODE}
#endif
"
    )
endfunction()

# TODO: consider renaming to okteta_add_classes
macro(okteta_add_sublibrary _baseName)
    set(options
        NO_VERSIONED_INCLUDEDIR
        REVERSE_NAMESPACE_INCLUDEDIR
        BUILD_INCLUDEDIR
    )
    set(oneValueArgs
        SUBDIR
        LIBRARY
    )
    set(multiValueArgs
        NAMESPACE
        PUBLIC
        PRIVATE
        KCFG
        UI
        QRC
    )
    cmake_parse_arguments(OKTETA_ADD_SUBLIBRARY "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    _okteta_target_name(_targetName ${_baseName} ${OKTETA_ADD_SUBLIBRARY_NAMESPACE})

    string(CONCAT _namespaceConcat ${OKTETA_ADD_SUBLIBRARY_NAMESPACE})
    string(TOLOWER "${_namespaceConcat}${_baseName}" _libname)
    if (OKTETA_ADD_SUBLIBRARY_LIBRARY)
        set(_library_target ${OKTETA_ADD_SUBLIBRARY_LIBRARY})
    else()
        set(_library_target ${_targetName})
    endif()
    if (OKTETA_ADD_SUBLIBRARY_SUBDIR)
        set(_relativePath "${OKTETA_ADD_SUBLIBRARY_SUBDIR}/")
        set(_egh_relative_param RELATIVE ${OKTETA_ADD_SUBLIBRARY_SUBDIR})
    else()
        set(_relativePath)
        set(_egh_relative_param)
    endif()
    set(_srcs )

    foreach(_classname ${OKTETA_ADD_SUBLIBRARY_PUBLIC} ${OKTETA_ADD_SUBLIBRARY_PRIVATE})
        string(TOLOWER "${_classname}" _lc_classname)

        set(_source "${_relativePath}${_lc_classname}.cpp")
        set(_actualsource "${CMAKE_CURRENT_SOURCE_DIR}/${_source}")
        if (EXISTS ${_actualsource})
            list(APPEND _srcs "${_source}")
        endif()

        set(_source "${_relativePath}${_lc_classname}_p.cpp")
        set(_actualsource "${CMAKE_CURRENT_SOURCE_DIR}/${_source}")
        if (EXISTS ${_actualsource})
            list(APPEND _srcs "${_source}")
        endif()
    endforeach()

    foreach(_kcfg ${OKTETA_ADD_SUBLIBRARY_KCFG})
        kconfig_add_kcfg_files(${_library_target} "${_relativePath}${_kcfg}")
    endforeach()

    foreach(_ui ${OKTETA_ADD_SUBLIBRARY_UI})
        ki18n_wrap_ui(${_library_target} "${_relativePath}${_ui}")
    endforeach()

    foreach(_qrc ${OKTETA_ADD_SUBLIBRARY_QRC})
        qt5_add_resources(_srcs "${_relativePath}${_qrc}")
    endforeach()

    target_sources(${_library_target} PRIVATE ${_srcs})
    if (OKTETA_ADD_SUBLIBRARY_PUBLIC)
        set(_cc_include_dir ${OKTETA_ADD_SUBLIBRARY_NAMESPACE})
        if (OKTETA_ADD_SUBLIBRARY_REVERSE_NAMESPACE_INCLUDEDIR)
            list(REVERSE _cc_include_dir)
        endif()
        string(REPLACE ";" "/" _cc_include_dir "${_cc_include_dir}")

        ecm_generate_headers(_cchdrs
            HEADER_NAMES
                ${OKTETA_ADD_SUBLIBRARY_PUBLIC}
            ${_egh_relative_param}
            HEADER_EXTENSION hpp
            PREFIX ${_cc_include_dir}
            REQUIRED_HEADERS _hdrs
        )
    endif()

    if (OKTETA_ADD_SUBLIBRARY_BUILD_INCLUDEDIR)
        target_include_directories( ${_library_target}
            PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${_relativePath}>
        )
    endif()

    # install
    get_property(_include_install_dir TARGET ${_targetName} PROPERTY OKTETA_INSTALL_INCLUDEDIR)
    if (_include_install_dir)
        get_property(_include_dir TARGET ${_targetName} PROPERTY OKTETA_INSTALL_NORMAL_HEADERS_SUBDIR)
        get_property(_cc_include_dir TARGET ${_targetName} PROPERTY OKTETA_INSTALL_CAMELCASE_HEADERS_SUBDIR)
        install( FILES ${_hdrs}
            DESTINATION "${_include_install_dir}/${_include_dir}"
            COMPONENT Devel
        )
        install( FILES ${_cchdrs}
            DESTINATION "${_include_install_dir}/${_cc_include_dir}"
            COMPONENT Devel
        )
    endif()
endmacro()


function(okteta_add_library _baseName)
    set(options
        NO_TARGET_NAMESPACE
        NO_VERSIONED_INCLUDEDIR
        NO_VERSIONED_PACKAGE_NAME
        NO_VERSIONED_EXPORTED_TARGET_NAME
        NO_PACKAGE_NAMESPACED_INCLUDEDIR
        REVERSE_NAMESPACE_LIB
        REVERSE_NAMESPACE_INCLUDEDIR
        LOWERCASE_LIB
        NAMELINK_SKIP
    )
    set(oneValueArgs
        INTERNAL_BASENAME
        INCLUDEDIR_PACKAGE_NAMESPACE
        VERSION
        SOVERSION
    )
    set(multiValueArgs
        NAMESPACE
        ABIVERSION
        PUBLIC
        PRIVATE
    )
    cmake_parse_arguments(OKTETA_ADD_LIBRARY "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT OKTETA_ADD_LIBRARY_NO_VERSIONED_PACKAGE_NAME)
        set(_use_versioned_package_name TRUE)
    endif()
    if (NOT OKTETA_ADD_LIBRARY_INTERNAL_BASENAME)
        set(OKTETA_ADD_LIBRARY_INTERNAL_BASENAME ${_baseName})
    endif()

    _okteta_setup_namespace(
        NAMESPACEPREFIX_VAR _namespacePrefix
        VERSIONEDNAMESPACEPREFIX_VAR _versionedNamespacePrefix
        VERSIONEDNAMESPACE_VAR _versioned_namespace
        BASE_NAME ${_baseName}
        NAMESPACE ${OKTETA_ADD_LIBRARY_NAMESPACE}
        ABIVERSION ${OKTETA_ADD_LIBRARY_ABIVERSION}
    )

    set(_fullName "${_namespacePrefix}${_baseName}")
    set(_fullInternalName "${_namespacePrefix}${OKTETA_ADD_LIBRARY_INTERNAL_BASENAME}")
    set(_fullVersionedName "${_versionedNamespacePrefix}${_baseName}")

    set(_targets_export_name "${_fullName}Targets")

    set(_libraryName ${_versioned_namespace})
    if (OKTETA_ADD_LIBRARY_REVERSE_NAMESPACE_LIB)
        list(REVERSE _libraryName)
    endif()
    string(CONCAT _libraryName ${_libraryName} ${_baseName})
    if (OKTETA_ADD_LIBRARY_LOWERCASE_LIB)
        string(TOLOWER ${_libraryName} _libraryName)
    endif()

    set(_targetName ${_fullName})
    string(TOLOWER ${_fullInternalName} _lc_fullInternalName)
    if (OKTETA_ADD_LIBRARY_NO_VERSIONED_INCLUDEDIR)
        set(_cc_include_dir ${OKTETA_ADD_LIBRARY_NAMESPACE})
    else()
        set(_cc_include_dir ${_versioned_namespace})
    endif()
    if (OKTETA_ADD_LIBRARY_REVERSE_NAMESPACE_INCLUDEDIR)
        list(REVERSE _cc_include_dir)
    endif()
    string(REPLACE ";" "/" _cc_include_dir "${_cc_include_dir}")
    string(TOLOWER ${_cc_include_dir} _include_dir)

    add_library(${_targetName} SHARED)

    if (OKTETA_ADD_LIBRARY_NO_TARGET_NAMESPACE)
        if (OKTETA_ADD_LIBRARY_NO_VERSIONED_EXPORTED_TARGET_NAME)
            set(_export_name_args)
        else()
            add_library("${_fullVersionedName}" ALIAS ${_targetName})
            set(_export_name_args EXPORT_NAME ${_fullVersionedName})
        endif()
    else()
        if (OKTETA_ADD_LIBRARY_NO_VERSIONED_EXPORTED_TARGET_NAME)
            set(_target_namespacePrefix ${_namespacePrefix})
        else()
            set(_target_namespacePrefix ${_versionedNamespacePrefix})
        endif()
        add_library("${_target_namespacePrefix}::${_baseName}" ALIAS ${_targetName})
        set(_export_name_args EXPORT_NAME ${_baseName})
    endif()

    # generate header file with export & version macros
    string(TOUPPER "${_fullInternalName}" _definitions_prefix)
    _okteta_generate_version_code(
        PREFIX        ${_definitions_prefix}
        VERSION       ${OKTETA_ADD_LIBRARY_VERSION}
        CODE_VARIABLE _version_code
    )
    _okteta_generate_export_code(
        TARGET_NAME   ${_targetName}
        PREFIX        ${_definitions_prefix}
        CODE_VARIABLE _export_code
    )

    set(_exportHeaderFileName ${_lc_fullInternalName}_export.hpp)
    set(_exportHeaderFilePath ${CMAKE_CURRENT_BINARY_DIR}/${_exportHeaderFileName})
    string(TOUPPER ${_exportHeaderFileName} _include_guard_name)
    string(MAKE_C_IDENTIFIER ${_include_guard_name} _include_guard_name)

    _okteta_generate_header_file(
        CODE "${_version_code}\n${_export_code}"
        INCLUDE_GUARD_NAME ${_include_guard_name}
        EXPORT_FILE_NAME ${_exportHeaderFilePath}
    )

    # Local forwarding header
    set(_forwardexportHeaderFilePath ${CMAKE_CURRENT_BINARY_DIR}/${_include_dir}/${_exportHeaderFileName})
    if (NOT EXISTS ${_forwardexportHeaderFilePath})
        file(WRITE ${_forwardexportHeaderFilePath} "#include \"${_exportHeaderFilePath}\"\n")
    endif()

    target_link_libraries(${_targetName}
        PUBLIC
            ${OKTETA_ADD_LIBRARY_PUBLIC}
        PRIVATE
            ${OKTETA_ADD_LIBRARY_PRIVATE}
    )

    target_include_directories(${_targetName}
        PUBLIC "$<BUILD_INTERFACE:${${_targetName}_BINARY_DIR};${CMAKE_CURRENT_BINARY_DIR}>"
    )

    set_target_properties(${_targetName} PROPERTIES
        ${_export_name_args}
        OUTPUT_NAME ${_libraryName}
        VERSION     ${OKTETA_ADD_LIBRARY_VERSION}
        SOVERSION   ${OKTETA_ADD_LIBRARY_SOVERSION}
    )
    set_property(TARGET ${_targetName} PROPERTY OKTETA_FULLNAME ${_fullName})
    set_property(TARGET ${_targetName} PROPERTY OKTETA_FULLVERSIONEDNAME ${_fullVersionedName})
    set_property(TARGET ${_targetName} PROPERTY OKTETA_NO_TARGETNAMESPACE ${OKTETA_ADD_LIBRARY_NO_TARGET_NAMESPACE})
    if (NOT OKTETA_ADD_LIBRARY_NO_TARGET_NAMESPACE)
        set_property(TARGET ${_targetName} PROPERTY OKTETA_TARGET_NAMESPACEPREFIX ${_target_namespacePrefix})
    endif()
    set_property(TARGET ${_targetName} PROPERTY OKTETA_USE_VERSIONED_PACKAGE_NAME ${_use_versioned_package_name})

    set(_other_install_targets_args)
    if(OKTETA_ADD_LIBRARY_NAMELINK_SKIP)
        list(APPEND _other_install_targets_args LIBRARY NAMELINK_SKIP)
    endif()
    install( TARGETS ${_targetName}
        EXPORT ${_targets_export_name}
        ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
        ${_other_install_targets_args}
    )

    # TODO: perhaps only do on first PUBLIC usage in okteta_add_sublibrary?
    if (OKTETA_ADD_LIBRARY_NO_PACKAGE_NAMESPACED_INCLUDEDIR)
        set(_include_install_dir "${KDE_INSTALL_INCLUDEDIR}")
    else()
        if (OKTETA_ADD_LIBRARY_INCLUDEDIR_PACKAGE_NAMESPACE)
            set(_include_dir_package_namespace "${OKTETA_ADD_LIBRARY_INCLUDEDIR_PACKAGE_NAMESPACE}")
        else()
            if (_use_versioned_package_name)
                set(_include_dir_package_namespace "${_fullVersionedName}")
            else()
                set(_include_dir_package_namespace "${_fullName}")
            endif()
        endif()
        set(_include_install_dir "${KDE_INSTALL_INCLUDEDIR}/${_include_dir_package_namespace}")
    endif()
    set_property(TARGET ${_targetName} PROPERTY OKTETA_INSTALL_INCLUDEDIR ${_include_install_dir})
    set_property(TARGET ${_targetName} PROPERTY OKTETA_INSTALL_NORMAL_HEADERS_SUBDIR ${_include_dir})
    set_property(TARGET ${_targetName} PROPERTY OKTETA_INSTALL_CAMELCASE_HEADERS_SUBDIR ${_cc_include_dir})
    target_include_directories(${_targetName}
        INTERFACE "$<INSTALL_INTERFACE:${_include_install_dir}>"
    )
    install( FILES ${_exportHeaderFilePath}
        DESTINATION "${_include_install_dir}/${_include_dir}"
        COMPONENT Devel
    )
endfunction()


function(okteta_add_cmakeconfig _baseName)
    set(options
    )
    set(oneValueArgs
        COMPATIBILITY
    )
    set(multiValueArgs
        INCLUDES
        NAMESPACE
        DEPS
        VARS
    )
    cmake_parse_arguments(OKTETA_ADD_CMAKECONFIG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    _okteta_target_name(_targetName ${_baseName} ${OKTETA_ADD_CMAKECONFIG_NAMESPACE})

    get_property(_fullName TARGET ${_targetName} PROPERTY OKTETA_FULLNAME)

    get_property(_use_versioned_package_name TARGET ${_targetName} PROPERTY OKTETA_USE_VERSIONED_PACKAGE_NAME)
    if (_use_versioned_package_name)
        get_property(_configName TARGET ${_targetName} PROPERTY OKTETA_FULLVERSIONEDNAME)
    else()
        set(_configName ${_fullName})
    endif()

    set(_configFileTemplatePath "${CMAKE_CURRENT_BINARY_DIR}/${_configName}Config.cmake.in")
    set(_configFileTemplate "@PACKAGE_INIT@\n\ninclude(CMakeFindDependencyMacro)\n\n")
    set(dep_package_name)
    foreach (dep ${OKTETA_ADD_CMAKECONFIG_DEPS})
        if (dep_package_name)
            set (dep_package_version ${dep})
        else()
            set(_dep_fullName)
            if (TARGET ${dep})
                get_property(_dep_fullName TARGET ${dep} PROPERTY OKTETA_FULLNAME)
            endif()
            if (_dep_fullName)
                get_property(_use_versioned_package_name TARGET ${dep} PROPERTY OKTETA_USE_VERSIONED_PACKAGE_NAME)
                if (_use_versioned_package_name)
                    get_property(dep_package_name TARGET ${dep} PROPERTY OKTETA_FULLVERSIONEDNAME)
                else()
                    set(dep_package_name ${_dep_fullName})
                endif()

                get_property(dep_package_version TARGET ${dep} PROPERTY VERSION)
            else()
                set(dep_package_name ${dep})
                set(dep_package_version)
            endif()
        endif()
        if (dep_package_version)
            string(APPEND _configFileTemplate "find_dependency(${dep_package_name} ${dep_package_version})\n")
            set(dep_package_name)
        endif()
    endforeach()
    if (dep_package_name)
        message(FATAL_ERROR "DEPS expected to have a version after ${dep_package_name}.")
    endif()
    string(APPEND _configFileTemplate "\ninclude(\"\${CMAKE_CURRENT_LIST_DIR}/${_configName}Targets.cmake\")\n")
    foreach (_include ${OKTETA_ADD_CMAKECONFIG_INCLUDES})
        string(APPEND _configFileTemplate "include(\"\${CMAKE_CURRENT_LIST_DIR}/${_include}\")\n")
    endforeach()
    if (OKTETA_ADD_CMAKECONFIG_VARS)
        string(APPEND _configFileTemplate "\n")
        foreach (_var ${OKTETA_ADD_CMAKECONFIG_VARS})
            string(APPEND _configFileTemplate "@${_var}@\n")
        endforeach()
    endif()
    file(WRITE ${_configFileTemplatePath} "${_configFileTemplate}")

    set(_targets_export_name "${_fullName}Targets")

    # create a Config.cmake and a ConfigVersion.cmake file and install them
    set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/${_configName}")

    get_target_property(_version ${_targetName} VERSION)
    if (_version)
        set(_configVersionFilePath "${CMAKE_CURRENT_BINARY_DIR}/${_configName}ConfigVersion.cmake")
        if(NOT OKTETA_ADD_CMAKECONFIG_COMPATIBILITY)
            set(OKTETA_ADD_CMAKECONFIG_COMPATIBILITY AnyNewerVersion)
        endif()
        write_basic_package_version_file("${_configVersionFilePath}"
            VERSION ${_version}
            COMPATIBILITY ${OKTETA_ADD_CMAKECONFIG_COMPATIBILITY}
        )
    else()
        set(_configVersionFilePath)
    endif()

    set(_configFilePath "${CMAKE_CURRENT_BINARY_DIR}/${_configName}Config.cmake")
    configure_package_config_file(
        "${_configFileTemplatePath}"
        "${_configFilePath}"
        INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    )

    install( FILES
            "${_configFilePath}"
            "${_configVersionFilePath}"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    get_property(_no_target_namespace TARGET ${_targetName} PROPERTY OKTETA_NO_TARGETNAMESPACE)
    if(NOT _no_target_namespace)
        get_property(_namespacePrefix TARGET ${_targetName} PROPERTY OKTETA_TARGET_NAMESPACEPREFIX)
        set(_namespace_args NAMESPACE "${_namespacePrefix}::")
    endif()
    install(EXPORT ${_targets_export_name}
        FILE "${_configName}Targets.cmake"
        ${_namespace_args}
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
endfunction()


function(okteta_add_qmakeconfig _baseName)
    set(options
    )
    set(oneValueArgs
    )
    set(multiValueArgs
        NAMESPACE
        DEPS
    )
    cmake_parse_arguments(OKTETA_ADD_QMAKECONFIG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    _okteta_target_name(_targetName ${_baseName} ${OKTETA_ADD_QMAKECONFIG_NAMESPACE})

    get_target_property(_libraryName ${_targetName} OUTPUT_NAME)
    get_property(_include_install_dir TARGET ${_targetName} PROPERTY OKTETA_INSTALL_INCLUDEDIR)
    get_property(_fullName TARGET ${_targetName} PROPERTY OKTETA_FULLNAME)
    get_property(_use_versioned_package_name TARGET ${_targetName} PROPERTY OKTETA_USE_VERSIONED_PACKAGE_NAME)
    if (_use_versioned_package_name)
        get_property(_configName TARGET ${_targetName} PROPERTY OKTETA_FULLVERSIONEDNAME)
    else()
        get_property(_configName TARGET ${_targetName} PROPERTY OKTETA_FULLNAME)
    endif()

    string(REPLACE ";" " " _deps "${OKTETA_ADD_QMAKECONFIG_DEPS}")

    get_target_property(_version ${_targetName} VERSION)

    ecm_generate_pri_file(
        BASE_NAME ${_configName}
        LIB_NAME ${_libraryName}
        DEPS ${_deps}
        VERSION ${_version}
        FILENAME_VAR _pri_filename
        INCLUDE_INSTALL_DIR  ${_include_install_dir}
    )
    install(FILES ${_pri_filename}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
        COMPONENT Devel
    )
endfunction()

function(okteta_add_pkgconfig _baseName)
    set(options
    )
    set(oneValueArgs
        DESCRIPTION
    )
    set(multiValueArgs
        NAMESPACE
        DEPS
    )
    cmake_parse_arguments(OKTETA_ADD_PKGCONFIG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    _okteta_target_name(_targetName ${_baseName} ${OKTETA_ADD_PKGCONFIG_NAMESPACE})

    get_target_property(_libraryName ${_targetName} OUTPUT_NAME)
    get_property(_include_install_dir TARGET ${_targetName} PROPERTY OKTETA_INSTALL_INCLUDEDIR)
    get_property(_use_versioned_package_name TARGET ${_targetName} PROPERTY OKTETA_USE_VERSIONED_PACKAGE_NAME)
    if (_use_versioned_package_name)
        get_property(_configName TARGET ${_targetName} PROPERTY OKTETA_FULLVERSIONEDNAME)
    else()
        get_property(_configName TARGET ${_targetName} PROPERTY OKTETA_FULLNAME)
    endif()

    string(REPLACE ";" " " _deps "${OKTETA_ADD_PKGCONFIG_DEPS}")

    get_target_property(PROJECT_VERSION ${_targetName} VERSION)
    ecm_generate_pkgconfig_file(
        BASE_NAME ${_configName}
        LIB_NAME ${_libraryName}
        DEPS ${_deps}
        INCLUDE_INSTALL_DIR  ${_include_install_dir}
        DESCRIPTION ${OKTETA_ADD_PKGCONFIG_DESCRIPTION}
        INSTALL
    )
endfunction()
