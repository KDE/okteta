cmake_minimum_required(VERSION 2.8.11.1)

project( okteta )

#copy the way kde-workspace does it

find_package(ECM 0.0.8 REQUIRED NO_MODULE)
# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

find_package(Qt5 REQUIRED NO_MODULE COMPONENTS Core Widgets Script ScriptTools Xml Network PrintSupport)
if (BUILD_TESTING)
  find_package(Qt5Test REQUIRED NO_MODULE)
endif()
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0) #allow deprecated functions for now

# Load CMake, Compiler and InstallDirs settings from KF5 and the following are already somewhat "done" tier1/tier2 libs from kdelibs:
find_package(KF5 MODULE REQUIRED COMPONENTS CMake Compiler InstallDirs
                                            KWidgetsAddons KCoreAddons KConfig KI18n KGuiAddons KDBusAddons KService ItemViews KAuth KCodecs
                                            KConfigWidgets KWindowSystem KNotifications KIconThemes Sonnet KCompletion 
                                            KTextWidgets XmlGui KBookmarks KDBusAddons KCrash KArchive ItemModels KJobWidgets Solid KWallet KJS
                                            KIO KUnitConversion KDE4Attic Kross KParts KCMUtils KPrintUtils
                                            )
if( UNIX )
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x -fno-rtti")
endif()

# needed for set_package_properties macro
find_package(KDELibs4 REQUIRED NO_MODULE) #needed for KDE4Support
find_package(KDE4Support REQUIRED NO_MODULE)

set(CMAKE_POSITION_INDEPENDENT_CODE ON) #this is needed since it is apparently not set by Qt5Config.cmake

set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC TRUE )
include_directories( ${KDE4_INCLUDES} )

include(FeatureSummary)

find_package(QCA2)
set_package_properties(QCA2 PROPERTIES DESCRIPTION "Qt Cryptographic Architecture"
                       URL "http://delta.affinix.com/qca" TYPE OPTIONAL
                       PURPOSE "Needed for most of the algorithms of the checksum tool")

# control build scope
# TODO: examples and templates not internal need headers and libs as installed
# so they can be only build after the installation was done
# How to handle this best, as in: they are per default always build after an install?
# set( KASTEN_BUILD_EXAMPLES  NOT OMIT_EXAMPLES )
# set( OKTETA_BUILD_EXAMPLES  NOT OMIT_EXAMPLES )
set( KASTEN_BUILD_INTERNAL_EXAMPLES  NOT OMIT_INTERNAL_EXAMPLES )
set( OKTETA_BUILD_INTERNAL_EXAMPLES  NOT OMIT_INTERNAL_EXAMPLES )

# set( KASTEN_BUILD_TEMPLATES NOT SKIPTEMPLATES )
# set( OKTETA_BUILD_TEMPLATES NOT SKIPTEMPLATES )
set( KASTEN_BUILD_INTERNAL_TEMPLATES NOT OMIT_INTERNAL_TEMPLATES )
set( OKTETA_BUILD_INTERNAL_TEMPLATES NOT OMIT_INTERNAL_TEMPLATES )

# mobile needs
option(OKTETA_MOBILE_UI "Build UI for mobile devices instead of for desktops" FALSE)
if (OKTETA_MOBILE_UI)
  find_package(Qt5Quick REQUIRED)
  add_definitions( -DOKTETA_MOBILE_UI )
endif (OKTETA_MOBILE_UI)

add_definitions(
    -DQT_USE_FAST_CONCATENATION
    -DQT_USE_FAST_OPERATOR_PLUS
)
# switch between no cast to/from ascii and a warning about it
if (KDE4_ASCII_CAST)
add_definitions(
    -DQT_ASCII_CAST_WARNINGS
)
else (KDE4_ASCII_CAST)
add_definitions(
    -DQT_NO_CAST_TO_ASCII
    -DQT_NO_CAST_FROM_ASCII
)
endif (KDE4_ASCII_CAST)

# helper libs
add_subdirectory( libs )
add_subdirectory( doc )

set( OKTETALIBS_MAJOR_VERSION 0 )
set( OKTETALIBS_MINOR_VERSION 8 )
set( OKTETALIBS_PATCHLEVEL_VERSION 1 )
set( OKTETALIBS_VERSION "${OKTETALIBS_MAJOR_VERSION}.${OKTETALIBS_MINOR_VERSION}.${OKTETALIBS_PATCHLEVEL_VERSION}" )
set( OKTETALIBS_ABI_VERSION 1 )
set( OKTETALIBS_INCLUDE_INSTALL_DIR  ${INCLUDE_INSTALL_DIR}/okteta${OKTETALIBS_ABI_VERSION} )
set( OKTETALIBS_CCINCLUDE_INSTALL_DIR  ${INCLUDE_INSTALL_DIR}/KDE/Okteta${OKTETALIBS_ABI_VERSION} )

set( oktetacore_LIB  oktetacore )
set( oktetagui_LIB   oktetagui )
set( oktetacore_LIB_NAME  okteta${OKTETALIBS_ABI_VERSION}core )
set( oktetagui_LIB_NAME   okteta${OKTETALIBS_ABI_VERSION}gui )

set( OKTETALIBS_LIB_SOVERSION ${OKTETALIBS_ABI_VERSION} )
set( OKTETALIBS_LIB_VERSION ${OKTETALIBS_VERSION} )

# Okteta libs
add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( designer )

# Okteta Kasten components
add_subdirectory( kasten )
# special mimetypes
#disable this for now, FindSharedMimeInfo is no longer installed by KF5
#TODO fix this
#add_subdirectory( mimetypes )

# programs and parts for desktop
add_subdirectory( parts )
add_subdirectory( program )

# program for mobile
if (Qt5Quick_FOUND AND OKTETA_MOBILE_UI)
    macro_optional_add_subdirectory(mobile)
endif ()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
