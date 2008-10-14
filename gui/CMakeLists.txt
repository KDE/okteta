project( oktetagui )

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=1802 )

include_directories(
  ../core
  ../gui/libcolumnsview
)

set( libcontroller_SRCS
  controller/kchareditor.cpp
  controller/kcontroller.cpp
  controller/keditor.cpp
  controller/knavigator.cpp
  controller/ktabcontroller.cpp
  controller/kvalueeditor.cpp
)

set( libcolumnsview_SRCS
  libcolumnsview/columnrenderer.cpp
  libcolumnsview/columnsview.cpp
)

set( oktetagui_LIB_SRCS
  ${libcolumnsview_SRCS}
  ${libcontroller_SRCS}
  bordercolumnrenderer.cpp
  offsetcolumnrenderer.cpp
  abstractbytearraycolumnrenderer.cpp
  valuebytearraycolumnrenderer.cpp
  charbytearraycolumnrenderer.cpp
  kbytearrayview.cpp
  koffsetformat.cpp
  kdatacursor.cpp
  bytearraytablelayout.cpp
  kdataranges.cpp
  kcursor.cpp
  coordrangelist.cpp
)

kde4_add_library( oktetagui  SHARED ${oktetagui_LIB_SRCS} )
target_link_libraries( oktetagui  oktetacore ${KDE4_KDEUI_LIBS} )
set_target_properties( oktetagui  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS oktetagui  ${INSTALL_TARGETS_DEFAULT_ARGS} )

add_subdirectory( test )
#   kde4_add_library( testoktetacore STATIC ${oktetagui_LIB_SRCS} )

