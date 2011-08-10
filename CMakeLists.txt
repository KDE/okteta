project( okteta )

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

# temporary check for broken code in the Structures tool with bigendianness
include(TestBigEndian)
test_big_endian(BIG_ENDIAN)
if (BIG_ENDIAN)
  set( NO_STRUCTURES_TOOL TRUE )
endif (BIG_ENDIAN)

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

set( OKTETALIBS_MAJOR_VERSION 0 )
set( OKTETALIBS_MINOR_VERSION 8 )
set( OKTETALIBS_PATCHLEVEL_VERSION 0 )
set( OKTETALIBS_VERSION "${OKTETALIBS_MAJOR_VERSION}.${OKTETALIBS_MINOR_VERSION}.${OKTETALIBS_PATCHLEVEL_VERSION}" )
set( OKTETALIBS_ABI_VERSION 1 )
set( OKTETALIBS_INCLUDE_INSTALL_DIR  ${INCLUDE_INSTALL_DIR}/okteta${OKTETALIBS_ABI_VERSION} )

set( oktetacore_LIB  okteta${OKTETALIBS_ABI_VERSION}core )
set( oktetagui_LIB  okteta${OKTETALIBS_ABI_VERSION}gui )

set( OKTETALIBS_LIB_SOVERSION ${OKTETALIBS_ABI_VERSION} )
set( OKTETALIBS_LIB_VERSION ${OKTETALIBS_VERSION} )

# Okteta libs
add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( includes )
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
