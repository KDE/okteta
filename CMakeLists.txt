project( okteta )

set( KASTEN_BUILD_EXAMPLES  NOT SKIPEXAMPLES )
set( KASTEN_BUILD_TEMPLATES NOT SKIPTEMPLATES )

add_subdirectory( libs )

add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( includes )

add_subdirectory( kasten )

add_subdirectory( parts )
add_subdirectory( designer )
add_subdirectory( program )
