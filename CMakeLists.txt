project( okteta )

cmake_minimum_required(VERSION 2.8.12)
set(QT_MIN_VERSION "5.2.0")

find_package(ECM 1.0.0 REQUIRED NO_MODULE)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules" ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(ECMInstallIcons)
include(ECMGenerateHeaders)
include(ECMSetupVersion)
include(ECMPackageConfigHelpers)

include(FeatureSummary)
include(GenerateExportHeader)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

find_package(Qt5 REQUIRED NO_MODULE
    COMPONENTS
    Core
    Network
    Designer
    PrintSupport
    Script
    ScriptTools
    Widgets
    Xml
)
find_package(Qt5Test QUIET)
if (NOT Qt5Test_FOUND)
  message(STATUS "Qt5Test not found, unit tests will not be built.")
endif()


find_package(KF5 REQUIRED
    COMPONENTS
    Bookmarks
    Codecs
    Completion
    ConfigWidgets
    DBusAddons
    DocTools
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
    -DQT_USE_QSTRINGBUILDER
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
    -DQT_NO_CAST_FROM_BYTEARRAY
)

# helper libs
add_subdirectory( libs )
add_subdirectory( doc )

set( OKTETALIBS_MAJOR_VERSION 0 )
set( OKTETALIBS_MINOR_VERSION 9 )
set( OKTETALIBS_PATCHLEVEL_VERSION 0 )
set( OKTETALIBS_VERSION "${OKTETALIBS_MAJOR_VERSION}.${OKTETALIBS_MINOR_VERSION}.${OKTETALIBS_PATCHLEVEL_VERSION}" )
set( OKTETALIBS_ABI_VERSION 2 )
set( OKTETALIBS_INCLUDE_INSTALL_DIR  ${INCLUDE_INSTALL_DIR}/okteta ) #${OKTETALIBS_ABI_VERSION} )
set( OKTETALIBS_CCINCLUDE_INSTALL_DIR  ${INCLUDE_INSTALL_DIR}/Okteta ) #${OKTETALIBS_ABI_VERSION} )

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
