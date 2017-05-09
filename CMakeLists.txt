cmake_minimum_required(VERSION 2.8.12)

project( okteta )

find_package(ECM 1.7.0 REQUIRED NO_MODULE)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

# ECM KDE macros (include first, to have their policies and settings effect all other macros)
include(KDEInstallDirs)
include(KDECMakeSettings NO_POLICY_SCOPE)
include(KDECompilerSettings NO_POLICY_SCOPE)
# ECM macros
include(ECMInstallIcons)
include(ECMGenerateHeaders)
include(ECMSetupVersion)
include(ECMPackageConfigHelpers)
include(ECMAddAppIcon)
# CMake macros
include(FeatureSummary)
include(GenerateExportHeader)


set(REQUIRED_QT_VERSION "5.2.0")
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED
    COMPONENTS
    Core
    Network
    PrintSupport
    Script
    ScriptTools
    Widgets
    Xml
)
find_package(Qt5Test ${REQUIRED_QT_VERSION} CONFIG QUIET)
if (NOT Qt5Test_FOUND)
  message(STATUS "Qt5Test not found, unit tests will not be built.")
endif()

find_package(Qt5Designer ${REQUIRED_QT_VERSION} CONFIG QUIET)
set_package_properties(Qt5Designer PROPERTIES TYPE OPTIONAL
    PURPOSE "Required to build Qt Designer plugins"
)
if(NOT Qt5Designer_VERSION_STRING VERSION_LESS 5.5.0)
    find_package(Qt5UiPlugin ${REQUIRED_QT_VERSION} CONFIG QUIET)
    set_package_properties(Qt5UiPlugin PROPERTIES TYPE OPTIONAL
        PURPOSE "Required to build Qt Designer plugins"
    )
    if (Qt5UiPlugin_FOUND)
        # for some reason, Qt5UiPlugin does not set its _INCLUDE_DIRS variable. Fill it manually
        get_target_property(Qt5UiPlugin_INCLUDE_DIRS Qt5::UiPlugin INTERFACE_INCLUDE_DIRECTORIES)
    endif()
    set(DesignerPluginDeps_FOUND ${Qt5UiPlugin_FOUND})
else()
    set(DesignerPluginDeps_FOUND ${Qt5Designer_FOUND})
endif()


set(REQUIRED_KF5_VERSION "5.7.0")
find_package(KF5 ${REQUIRED_KF5_VERSION} REQUIRED
    COMPONENTS
    Bookmarks
    Codecs
    Completion
    ConfigWidgets
    Crash
    DBusAddons
    DocTools
    IconThemes
    I18n
    KCMUtils
    KIO
    NewStuff
    Parts
    Service
    WidgetsAddons
    XmlGui
)

if( UNIX )
  add_compile_options(-Wall)
  # enable this to reduce the file sizes (might crash)
  # add_compile_options(-fno-rtti)
endif()

find_package(Qca-qt5 2.1.0)
set_package_properties(Qca-qt5 PROPERTIES DESCRIPTION "Qt Cryptographic Architecture"
                       URL "http:/download.kde.org/stable/qca-qt5" TYPE OPTIONAL
                       PURPOSE "Needed for most of the algorithms of the checksum tool")

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

add_definitions(
    -DQT_DEPRECATED_WARNINGS
    -DQT_DISABLE_DEPRECATED_BEFORE=0x050200
    -DQT_USE_QSTRINGBUILDER
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_FROM_BYTEARRAY
    -DQT_STRICT_ITERATORS
    -DQT_NO_URL_CAST_FROM_STRING
    -DQT_NO_SIGNALS_SLOTS_KEYWORDS
)

# helper libs
add_subdirectory( libs )
add_subdirectory( doc )

set( OKTETALIBS_MAJOR_VERSION 0 )
set( OKTETALIBS_MINOR_VERSION 9 )
set( OKTETALIBS_PATCHLEVEL_VERSION 1 )
set( OKTETALIBS_VERSION "${OKTETALIBS_MAJOR_VERSION}.${OKTETALIBS_MINOR_VERSION}.${OKTETALIBS_PATCHLEVEL_VERSION}" )
set( OKTETALIBS_ABI_VERSION 2 )
set( OKTETALIBS_INCLUDE_INSTALL_DIR  ${KDE_INSTALL_INCLUDEDIR}/okteta ) #${OKTETALIBS_ABI_VERSION} )
set( OKTETALIBS_CCINCLUDE_INSTALL_DIR  ${KDE_INSTALL_INCLUDEDIR}/Okteta ) #${OKTETALIBS_ABI_VERSION} )

set( OKTETALIBS_LIB_SOVERSION ${OKTETALIBS_ABI_VERSION} )
set( OKTETALIBS_LIB_VERSION ${OKTETALIBS_VERSION} )

# Okteta libs
add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( designer )

# Okteta Kasten components
add_subdirectory( kasten )
# special mimetypes
add_subdirectory( mimetypes )

# programs and parts for desktop
add_subdirectory( parts )
add_subdirectory( program )

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
