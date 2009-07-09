project( oktetakasten )

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=1806 )

add_subdirectory( document )
add_subdirectory( io )
add_subdirectory( view )
add_subdirectory( controllers )
