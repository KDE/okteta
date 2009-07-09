project( libframesprint )


set( LIBFRAMESPRINT_SRCS
  abstractframerenderer.cpp
  framestopaperprinter.cpp
  headerfooterframerenderer.cpp
)

kde4_add_library( framesprint STATIC ${LIBFRAMESPRINT_SRCS} )
