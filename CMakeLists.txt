cmake_minimum_required(VERSION 3.0)

project(Okteta VERSION 0.26.6)

set(REQUIRED_KF5_VERSION "5.48.0")
find_package(ECM ${REQUIRED_KF5_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${ECM_MODULE_PATH})

# ECM KDE macros (include first, to have their policies and settings effect all other macros)
include(KDEInstallDirs)
include(KDECMakeSettings NO_POLICY_SCOPE)
include(KDECompilerSettings NO_POLICY_SCOPE)
# ECM macros
include(ECMInstallIcons)
include(ECMAddAppIcon)
# CMake macros
include(FeatureSummary)
include(CMakeDependentOption)
if(BUILD_TESTING)
  include(ECMAddTests)
endif()
# Okteta macros
include(OktetaInternalMacros)

# control build scope, default to examples if BUILD_TESTING is ON
if( NOT DEFINED OMIT_EXAMPLES AND NOT BUILD_TESTING )
    set( OMIT_EXAMPLES TRUE )
endif()
if( NOT DEFINED OMIT_INTERNAL_TEMPLATES AND NOT BUILD_TESTING )
    set( OMIT_INTERNAL_TEMPLATES TRUE )
endif()

if( NOT OMIT_EXAMPLES )
    set( OKTETA_BUILD_EXAMPLES TRUE )
endif()
if( NOT OMIT_INTERNAL_TEMPLATES )
    set( KASTEN_BUILD_INTERNAL_TEMPLATES TRUE )
endif()

option(BUILD_DESIGNERPLUGIN "Build the Designer plugin." ON)
add_feature_info(DESIGNERPLUGIN ${BUILD_DESIGNERPLUGIN} "Qt Designer plugin")
option(BUILD_OKTETAKASTENLIBS "Build the Okteta Kasten libraries." ON)
add_feature_info(OKTETAKASTENLIBS ${BUILD_OKTETAKASTENLIBS} "Okteta Kasten libraries")

cmake_dependent_option(BUILD_KPARTSPLUGIN "Build the KParts plugin." ON "BUILD_OKTETAKASTENLIBS" OFF)
add_feature_info(KPARTSPLUGIN ${BUILD_KPARTSPLUGIN} "KParts plugin")
cmake_dependent_option(BUILD_DESKTOPPROGRAM "Build the program." ON "BUILD_OKTETAKASTENLIBS" OFF)
add_feature_info(DESKTOPPROGRAM ${BUILD_DESKTOPPROGRAM} "Program")

set(REQUIRED_QT_VERSION "5.9.0")
set(ADDITIONAL_REQUIRED_QT_COMPONENTS)
if(BUILD_TESTING)
    list(APPEND ADDITIONAL_REQUIRED_QT_COMPONENTS Test)
endif()
if( BUILD_DESIGNERPLUGIN )
    list(APPEND ADDITIONAL_REQUIRED_QT_COMPONENTS UiPlugin)
endif()
if( BUILD_OKTETAKASTENLIBS )
    list(APPEND ADDITIONAL_REQUIRED_QT_COMPONENTS
        Network
        PrintSupport
        Qml
        Script
        ScriptTools
        Xml
    )
endif()
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED
    COMPONENTS
    Core
    Widgets
    ${ADDITIONAL_REQUIRED_QT_COMPONENTS}
)

set(ADDITIONAL_REQUIRED_KF_COMPONENTS)
if( BUILD_OKTETAKASTENLIBS )
    list(APPEND ADDITIONAL_REQUIRED_KF_COMPONENTS
        Bookmarks
        Completion
        IconThemes
        KCMUtils
        KIO
        NewStuff
        Parts
        Service
        WidgetsAddons
        XmlGui
    )
endif()
if( BUILD_DESKTOPPROGRAM )
    list(APPEND ADDITIONAL_REQUIRED_KF_COMPONENTS
        Crash
        DBusAddons
        DocTools
    )
endif()
find_package(KF5 ${REQUIRED_KF5_VERSION} REQUIRED
    COMPONENTS
    Codecs
    ConfigWidgets
    I18n
    ${ADDITIONAL_REQUIRED_KF_COMPONENTS}
)

find_package(Qca-qt5 2.1.0)
set_package_properties(Qca-qt5 PROPERTIES
    DESCRIPTION "Qt Cryptographic Architecture"
    URL "https://commits.kde.org/qca"
    TYPE OPTIONAL
    PURPOSE "Needed for most of the algorithms of the checksum tool"
)

add_definitions(
    -DQT_DEPRECATED_WARNINGS
    -DQT_DISABLE_DEPRECATED_BEFORE=0x050900
    -DQT_USE_QSTRINGBUILDER
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_STRICT_ITERATORS
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_NO_SIGNALS_SLOTS_KEYWORDS
    -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT
)

# helper libs
add_subdirectory( libs )
if( BUILD_DESKTOPPROGRAM )
    add_subdirectory( doc )
endif()

set( OKTETALIBS_MAJOR_VERSION 0 )
set( OKTETALIBS_MINOR_VERSION 10 )
set( OKTETALIBS_PATCHLEVEL_VERSION 0 )
set( OKTETALIBS_VERSION "${OKTETALIBS_MAJOR_VERSION}.${OKTETALIBS_MINOR_VERSION}.${OKTETALIBS_PATCHLEVEL_VERSION}" )
set( OKTETALIBS_ABI_VERSION 3 )

set( OKTETALIBS_LIB_SOVERSION 0 )
set( OKTETALIBS_LIB_VERSION ${OKTETALIBS_VERSION} )

# Okteta libs
add_subdirectory( core )
add_subdirectory( gui )

if( BUILD_DESIGNERPLUGIN )
    add_subdirectory( designer )
endif()

# Okteta Kasten components
if( BUILD_OKTETAKASTENLIBS )
    add_subdirectory( kasten )
endif()
# special mimetypes
if( BUILD_OKTETAKASTENLIBS )
    add_subdirectory( mimetypes )
endif()

# programs and parts for desktop
add_subdirectory( parts )
if( BUILD_DESKTOPPROGRAM )
    add_subdirectory( program )
endif()

ki18n_install(po)

if( BUILD_DESKTOPPROGRAM )
    kdoctools_install(po)
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
