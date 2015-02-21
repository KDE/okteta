project( oktetagui )

add_definitions(-DTRANSLATION_DOMAIN=\"liboktetagui\")

include_directories(
  ../core
  libcolumnsview
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
)

ecm_generate_headers(libcolumnsview_CCHDRS
  HEADER_NAMES
    AbstractColumnStylist
    AbstractColumnRenderer
    ColumnsView
  RELATIVE libcolumnsview
  PREFIX Okteta
  REQUIRED_HEADERS libcolumnsview_HDRS
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
  oktetagui.cpp
)

set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/kcursor.cpp PROPERTIES LABELS OktetaLabel)

set( oktetagui_LIB_HDRS
  ${libcolumnsview_HDRS}
  ${CMAKE_CURRENT_BINARY_DIR}/oktetagui_export.h
)

set(oktetagui_LIB_CCHDRS
    ${libcolumnsview_CCHDRS}
)

ecm_generate_headers(oktetagui_LIB_CCHDRS
  HEADER_NAMES
    OktetaGui

    LinePosition
    Line
    LinePositionRange
    LineRange

    Coord
    CoordRange
    CoordRangeList
    Selection

    AbstractByteArrayColumnRenderer
    CharByteArrayColumnRenderer
    ValueByteArrayColumnRenderer
    ByteArrayRowColumnRenderer
    OffsetColumnRenderer
    BorderColumnRenderer

    AbstractByteArrayView
    ByteArrayColumnView

    OffsetFormat
    ByteArrayTableRanges
    ByteArrayRowView
    ByteArrayTableLayout
    WidgetColumnStylist
  PREFIX Okteta
  REQUIRED_HEADERS oktetagui_LIB_HDRS
)

add_library( ${oktetagui_LIB}  SHARED ${oktetagui_LIB_SRCS} )
target_link_libraries( ${oktetagui_LIB}
PUBLIC
  ${oktetacore_LIB}
  Qt5::Widgets
)

generate_export_header( ${oktetagui_LIB} )
target_include_directories( ${oktetagui_LIB} PUBLIC ${CMAKE_CURRENT_BINARY_DIR} )

target_link_libraries( ${oktetagui_LIB} LINK_PRIVATE
  KF5::I18n
  KF5::ConfigWidgets
)
set_target_properties( ${oktetagui_LIB}  PROPERTIES
  OUTPUT_NAME ${oktetagui_LIB_NAME}
  VERSION     ${OKTETALIBS_LIB_VERSION}
  SOVERSION   ${OKTETALIBS_LIB_SOVERSION}
  LABELS OktetaLabel
)

install( TARGETS ${oktetagui_LIB}  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${oktetagui_LIB_HDRS}
  DESTINATION ${OKTETALIBS_INCLUDE_INSTALL_DIR}
  COMPONENT Devel
)
install( FILES ${oktetagui_LIB_CCHDRS}
  DESTINATION ${OKTETALIBS_CCINCLUDE_INSTALL_DIR}
  COMPONENT Devel
)


add_subdirectory( test )
#   add_library( testoktetacore STATIC ${oktetagui_LIB_SRCS} )

