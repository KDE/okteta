cmake_minimum_required(VERSION 3.18)

project(Okteta VERSION 0.26.60)

set(REQUIRED_KF_VERSION "5.100.0")
find_package(ECM ${REQUIRED_KF_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${ECM_MODULE_PATH})

# setup use of ECMAddTest copy with newer features
if(ECM_VERSION VERSION_LESS 6.22)
    list(PREPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/ecmcopy")
elseif(REQUIRED_KF_VERSION VERSION_GREATER_EQUAL 6.22)
    message(WARNING "Remove copy of ECMAddTest, min. required ECM now provides it.")
endif()

# ECM KDE macros (include first, to have their policies and settings effect all other macros)
include(KDEInstallDirs)
include(KDECMakeSettings NO_POLICY_SCOPE)
include(KDECompilerSettings NO_POLICY_SCOPE)
# ECM macros
include(ECMQtDeclareLoggingCategory)
include(ECMInstallIcons)
include(ECMAddAppIcon)
include(ECMDeprecationSettings)
# CMake macros
include(FeatureSummary)
include(CMakeDependentOption)
if(BUILD_TESTING)
    include(ECMAddTests)
    set(ECM_TEST_NAME_PREFIX_STRATEGY "PATH")
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
cmake_dependent_option(BUILD_TERMINALTOOL "Build the Terminal tool (needs Konsole/Qt5 KPart)." ON "BUILD_OKTETAKASTENLIBS" OFF)
add_feature_info(TERMINALTOOL ${BUILD_TERMINALTOOL} "Terminal tool")

cmake_dependent_option(BUILD_KPARTSPLUGIN "Build the KParts plugin." ON "BUILD_OKTETAKASTENLIBS" OFF)
add_feature_info(KPARTSPLUGIN ${BUILD_KPARTSPLUGIN} "KParts plugin")
cmake_dependent_option(BUILD_DESKTOPPROGRAM "Build the program." ON "BUILD_OKTETAKASTENLIBS" OFF)
add_feature_info(DESKTOPPROGRAM ${BUILD_DESKTOPPROGRAM} "Program")

set(REQUIRED_QT_VERSION "5.15.2")
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
        Archive
        Bookmarks
        Completion
        ItemViews
        KIO
        NewStuff
        Service
        WidgetsAddons
        XmlGui
    )
endif()
if (BUILD_TERMINALTOOL OR BUILD_KPARTSPLUGIN)
    list(APPEND ADDITIONAL_REQUIRED_KF_COMPONENTS
        Parts
    )
endif()
if( BUILD_DESKTOPPROGRAM )
    list(APPEND ADDITIONAL_REQUIRED_KF_COMPONENTS
        Crash
        DBusAddons
        DocTools
    )
endif()
find_package(KF5 ${REQUIRED_KF_VERSION} REQUIRED
    COMPONENTS
    ConfigWidgets
    I18n
    ${ADDITIONAL_REQUIRED_KF_COMPONENTS}
)

ecm_set_disabled_deprecation_versions(
    # Disable Qt deprecation warnings as there should be no new deprecations in Qt5,
    # and QLinkedList porting will take some time, also not blocking Qt6 port
    DISABLE_NEWER_WARNINGS
    QT 5.14.0 # QLinkedList
)

ecm_set_disabled_deprecation_versions(
    KF 5.100
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Not setting for GNU due to too many warnings related to private members of base classes or around lambdas
    # see e.g. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56556 or https://gcc.gnu.org/bugzilla/show_bug.cgi?id=79328
    string(APPEND CMAKE_CXX_FLAGS " -Wshadow")
endif()

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

ecm_qt_install_logging_categories(
    EXPORT OKTETA
    FILE okteta.categories
    DESTINATION "${KDE_INSTALL_LOGGINGCATEGORIESDIR}"
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
