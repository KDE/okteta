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


add_subdirectory( libs )

add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( includes )

add_subdirectory( kasten )
add_subdirectory( mimetypes )

add_subdirectory( parts )
add_subdirectory( designer )
add_subdirectory( program )
