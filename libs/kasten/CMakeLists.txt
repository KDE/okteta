set( KASTEN_MAJOR_VERSION 0 )
set( KASTEN_MINOR_VERSION 4 )
set( KASTEN_PATCHLEVEL_VERSION 1 )
set( KASTEN_VERSION "${KASTEN_MAJOR_VERSION}.${KASTEN_MINOR_VERSION}.${KASTEN_PATCHLEVEL_VERSION}" )
set( KASTEN_ABI_VERSION 4 )

set( KASTEN_LIB_SOVERSION 0 )
set( KASTEN_LIB_VERSION ${KASTEN_VERSION} )

add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( controllers )

#add_subdirectory( kparts )
