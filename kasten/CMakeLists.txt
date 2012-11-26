project( oktetakasten )

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=1806 )

set( KASTEN_ABI_VERSION 2 )
# use the real lib names here once the base Kasten libs are moved into an own project
set( KASTENCORE_LIBS         kastencore )
set( KASTENGUI_LIBS          kastengui )
set( KASTENCONTROLLERS_LIBS  kastencontrollers )


set( OKTETAKASTEN_MAJOR_VERSION 0 )
set( OKTETAKASTEN_MINOR_VERSION 2 )
set( OKTETAKASTEN_PATCHLEVEL_VERSION 1 )
set( OKTETAKASTEN_VERSION "${OKTETAKASTEN_MAJOR_VERSION}.${OKTETAKASTEN_MINOR_VERSION}.${OKTETAKASTEN_PATCHLEVEL_VERSION}" )
set( OKTETAKASTEN_ABI_VERSION 1 )
set( OKTETAKASTEN_INCLUDE_INSTALL_DIR  ${INCLUDE_INSTALL_DIR}/kasten${KASTEN_ABI_VERSION}/okteta${OKTETAKASTEN_ABI_VERSION} )

set( oktetakastencore_LIB         oktetakastencore )
set( oktetakastengui_LIB          oktetakastengui )
set( oktetakastencontrollers_LIB  oktetakastencontrollers )
set( oktetakastencore_LIB_NAME         kasten${KASTEN_ABI_VERSION}okteta${OKTETAKASTEN_ABI_VERSION}core )
set( oktetakastengui_LIB_NAME          kasten${KASTEN_ABI_VERSION}okteta${OKTETAKASTEN_ABI_VERSION}gui )
set( oktetakastencontrollers_LIB_NAME  kasten${KASTEN_ABI_VERSION}okteta${OKTETAKASTEN_ABI_VERSION}controllers )

set( OKTETAKASTEN_LIB_SOVERSION ${OKTETAKASTEN_ABI_VERSION} )
set( OKTETAKASTEN_LIB_VERSION ${OKTETAKASTEN_VERSION} )


add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( controllers )
