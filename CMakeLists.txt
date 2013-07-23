cmake_minimum_required(VERSION 2.8.11.1)

project( okteta )

#copy the way kde-workspace does it

set(QT_MIN_VERSION "5.2.0")
find_package(ECM 0.0.8 REQUIRED NO_MODULE)
include(CMakePackageConfigHelpers)
# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

find_package(Qt5 REQUIRED NO_MODULE COMPONENTS Core Widgets Script ScriptTools)
find_package(Qt5Transitional MODULE)
#workaround wrong dirs being used

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0) #allow deprecated functions for now


find_package(kdeqt5staging REQUIRED NO_MODULE)
# Load CMake, Compiler and InstallDirs settings from KF5 and the following are already somewhat "done" tier1/tier2 libs from kdelibs:
find_package(KF5 MODULE REQUIRED COMPONENTS CMake Compiler InstallDirs
                                            KWidgetsAddons KCoreAddons KConfig KI18n KGuiAddons KService ItemViews KConfigWidgets 
                                            KWindowSystem KNotifications KAuth KCodecs KWidgets KIconThemes Sonnet KCompletion
                                            KTextWidgets XmlGui KDBusAddons KCrash KArchive ItemModels KJobWidgets Solid KWallet KJS
                                            )

# needed for set_package_properties macro
find_package(LibAttica REQUIRED NO_MODULE ) #somehow this is not searched for
find_package(KDELibs4 REQUIRED NO_MODULE) #needed for KDE4Support
find_package(KDE4Support REQUIRED NO_MODULE)
find_package(KDE4Attic REQUIRED NO_MODULE)

set(CMAKE_POSITION_INDEPENDENT_CODE ON) #this is needed since it is apparently not set by Qt5Config.cmake
#don't add KDE4Support manually to every target
set(KDE4_KDEUI_LIBS ${KDE4_KDEUI_LIBS} ${KDE4Support_LIBRARIES})


SET( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC TRUE )
include_directories( ${KDE4_INCLUDES} )

macro_optional_find_package( QCA2 )
macro_log_feature( QCA2_FOUND "QCA2" "Qt Cryptographic Architecture" "http://delta.affinix.com/qca" FALSE "2.0.0" "Needed for most of the algorithms of the checksum tool." )

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
  macro_log_feature(QT_QTDECLARATIVE_FOUND "QtDeclarative" "Qt Declarative (QML) module" "http://qt.nokia.com" TRUE "4.7" "Required for building mobile UI")
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
add_subdirectory( mimetypes )

# programs and parts for desktop
add_subdirectory( parts )
add_subdirectory( program )

# program for mobile
if (QT_QTDECLARATIVE_FOUND AND OKTETA_MOBILE_UI)
    macro_optional_add_subdirectory(mobile)
endif (QT_QTDECLARATIVE_FOUND AND OKTETA_MOBILE_UI)

macro_display_feature_log()
