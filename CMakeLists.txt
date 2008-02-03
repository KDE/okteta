project( okteta )

find_package( KDE4 REQUIRED )
include( KDE4Defaults )

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( parts )
add_subdirectory( program )
