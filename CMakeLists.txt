project( okteta )

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )
# include_directories (${QDBUS_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( parts )
add_subdirectory( program )
