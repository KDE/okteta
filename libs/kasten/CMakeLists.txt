project( kasten )

set( KASTEN_MAJOR_VERSION 0 )
set( KASTEN_MINOR_VERSION 3 )
set( KASTEN_PATCHLEVEL_VERSION 0 )
set( KASTEN_VERSION "${KASTEN_MAJOR_VERSION}.${KASTEN_MINOR_VERSION}.${KASTEN_PATCHLEVEL_VERSION}" )
set( KASTEN_ABI_VERSION 3 )
set( KASTEN_INCLUDE_INSTALL_DIR  ${INCLUDE_INSTALL_DIR}/kasten ) #${KASTEN_ABI_VERSION} )
set( KASTEN_CCINCLUDE_INSTALL_DIR  ${INCLUDE_INSTALL_DIR}/Kasten ) #${KASTEN_ABI_VERSION}

set( kastengui_LIB          kastengui )
set( kastencontrollers_LIB  kastencontrollers )
set( kastengui_LIB_NAME         kasten${KASTEN_ABI_VERSION}gui )
set( kastencontrollers_LIB_NAME kasten${KASTEN_ABI_VERSION}controllers )

set( KASTEN_LIB_SOVERSION ${KASTEN_ABI_VERSION} )
set( KASTEN_LIB_VERSION ${KASTEN_VERSION} )

add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( controllers )

#add_subdirectory( kparts )
