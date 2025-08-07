set( KASTEN_LIB_VERSION 0.4.0 )
set( KASTEN_ABI_VERSION 4 )

set( OKTETAKASTEN_MAJOR_VERSION 0 )
set( OKTETAKASTEN_MINOR_VERSION 4 )
set( OKTETAKASTEN_PATCHLEVEL_VERSION 0 )
set( OKTETAKASTEN_VERSION "${OKTETAKASTEN_MAJOR_VERSION}.${OKTETAKASTEN_MINOR_VERSION}.${OKTETAKASTEN_PATCHLEVEL_VERSION}" )
set( OKTETAKASTEN_ABI_VERSION 2 )

set( OKTETAKASTEN_LIB_SOVERSION 0 )
set( OKTETAKASTEN_LIB_VERSION ${OKTETAKASTEN_VERSION} )

find_package(LibLZMA)
set_package_properties(LibLZMA PROPERTIES
    TYPE REQUIRED
    PURPOSE "xz compressed stream encoding"
)


find_package(zstd 1.4)
set_package_properties(zstd PROPERTIES
    TYPE REQUIRED
    PURPOSE "zstd compressed stream encoding"
)

add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( controllers )
