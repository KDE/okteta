project( oktetakasten )

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=1806 )

add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( controllers )
