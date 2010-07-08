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


# helper libs
add_subdirectory( libs )

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
