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
  controller/abstractwheelcontroller.cpp
  controller/zoomwheelcontroller.cpp
  controller/dropper.cpp
)

set( libcolumnsview_SRCS
  libcolumnsview/abstractcolumnstylist.cpp
  libcolumnsview/abstractcolumnrenderer.cpp
  libcolumnsview/columnsview.cpp
)

set( libabstractbytearrayview_SRCS
  widgetcolumnstylist.cpp
  abstractbytearrayview_p.cpp
  abstractbytearrayview.cpp
)

set( bytearrayrowview_SRCS
  bytearrayrowcolumnrenderer.cpp
  bytearrayrowview_p.cpp
  bytearrayrowview.cpp
)

set( bytearraycolumnview_SRCS
  bytearraycolumnview_p.cpp
  bytearraycolumnview.cpp
)

set( oktetagui_LIB_SRCS
  ${libcolumnsview_SRCS}
  ${libcontroller_SRCS}
  ${libabstractbytearrayview_SRCS}
  bordercolumnrenderer.cpp
  offsetcolumnrenderer.cpp
  abstractbytearraycolumnrenderer.cpp
  valuebytearraycolumnrenderer.cpp
  charbytearraycolumnrenderer.cpp
  offsetformat.cpp
  bytearraytablecursor.cpp
  bytearraytablelayout.cpp
  bytearraytableranges.cpp
  kcursor.cpp
  coordrangelist.cpp
  ${bytearraycolumnview_SRCS}
  ${bytearrayrowview_SRCS}
)

kde4_add_library( oktetagui  SHARED ${oktetagui_LIB_SRCS} )
target_link_libraries( oktetagui  oktetacore ${KDE4_KDEUI_LIBS} )
set_target_properties( oktetagui  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS oktetagui  ${INSTALL_TARGETS_DEFAULT_ARGS} )

add_subdirectory( test )
#   kde4_add_library( testoktetacore STATIC ${oktetagui_LIB_SRCS} )

