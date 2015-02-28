project( oktetakasten )
cmake_minimum_required(VERSION 2.8.8)

set( KASTEN_ABI_VERSION 3 )

set( OKTETAKASTEN_MAJOR_VERSION 0 )
set( OKTETAKASTEN_MINOR_VERSION 3 )
set( OKTETAKASTEN_PATCHLEVEL_VERSION 0 )
set( OKTETAKASTEN_VERSION "${OKTETAKASTEN_MAJOR_VERSION}.${OKTETAKASTEN_MINOR_VERSION}.${OKTETAKASTEN_PATCHLEVEL_VERSION}" )
set( OKTETAKASTEN_ABI_VERSION 1 )
set( OKTETAKASTEN_INCLUDE_INSTALL_DIR  ${INCLUDE_INSTALL_DIR}/kasten/okteta ) #${KASTEN_ABI_VERSION}/okteta${OKTETAKASTEN_ABI_VERSION} )

set( oktetakastengui_LIB          oktetakastengui )
set( oktetakastencontrollers_LIB  oktetakastencontrollers )
set( oktetakastengui_LIB_NAME          kasten${KASTEN_ABI_VERSION}okteta${OKTETAKASTEN_ABI_VERSION}gui )
set( oktetakastencontrollers_LIB_NAME  kasten${KASTEN_ABI_VERSION}okteta${OKTETAKASTEN_ABI_VERSION}controllers )

set( OKTETAKASTEN_LIB_SOVERSION ${OKTETAKASTEN_ABI_VERSION} )
set( OKTETAKASTEN_LIB_VERSION ${OKTETAKASTEN_VERSION} )


add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( controllers )
