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
  controller/abstractmousecontroller.cpp
  controller/mousenavigator.cpp
  controller/mousepaster.cpp
#   controller/dragger.cpp
  controller/abstractwheelcontroller.cpp
  controller/zoomwheelcontroller.cpp
  controller/dropper.cpp
)

set( libcolumnsview_SRCS
  libcolumnsview/abstractcolumnstylist.cpp
  libcolumnsview/abstractcolumnrenderer.cpp
  libcolumnsview/columnsview.cpp
)

set( libcolumnsview_HDRS
  libcolumnsview/kadds.h
  libcolumnsview/abstractcolumnstylist.h
  libcolumnsview/abstractcolumnrenderer.h
  libcolumnsview/columnsview.h
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

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/kcursor.cpp PROPERTIES LABELS OktetaLabel)

set( oktetagui_LIB_HDRS
  ${libcolumnsview_HDRS}
  oktetagui_export.h
  oktetagui.h
  lineposition.h
  line.h
  linepositionrange.h
  linerange.h
  abstractbytearraycolumnrenderer.h
  offsetcolumnrenderer.h
  bordercolumnrenderer.h
  bytearrayrowcolumnrenderer.h
  charbytearraycolumnrenderer.h
  valuebytearraycolumnrenderer.h
  widgetcolumnstylist.h
  abstractbytearrayview.h
  bytearraycolumnview.h
  bytearrayrowview.h
  bytearraytablelayout.h 
  bytearraytableranges.h
  offsetformat.h
)

kde4_add_library( ${oktetagui_LIB}  SHARED ${oktetagui_LIB_SRCS} )
target_link_libraries( ${oktetagui_LIB}
  ${oktetacore_LIB}
  ${KDE4_KDEUI_LIBS}
)
set_target_properties( ${oktetagui_LIB}  PROPERTIES
  VERSION ${OKTETALIBS_LIB_VERSION}
  SOVERSION ${OKTETALIBS_LIB_SOVERSION}
  LABELS OktetaLabel
)

install( TARGETS ${oktetagui_LIB}  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${oktetagui_LIB_HDRS}
  DESTINATION ${OKTETALIBS_INCLUDE_INSTALL_DIR}
  COMPONENT Devel
)


add_subdirectory( test )
#   kde4_add_library( testoktetacore STATIC ${oktetagui_LIB_SRCS} )

