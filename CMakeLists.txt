project( okteta )

set( LIBFRAMESPRINT_SRCS
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/framesprint/abstractframerenderer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/framesprint/framestopaperprinter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/libs/framesprint/headerfooterframerenderer.cpp
)


add_subdirectory( libs )

add_subdirectory( core )
add_subdirectory( gui )
add_subdirectory( kasten )
add_subdirectory( parts )
add_subdirectory( program )
