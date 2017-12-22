ecm_setup_version( ${OKTETALIBS_LIB_VERSION}
    VARIABLE_PREFIX OKTETAGUI
    SOVERSION ${OKTETALIBS_LIB_SOVERSION}
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/OktetaGuiConfigVersion.cmake"
)

add_definitions(-DTRANSLATION_DOMAIN=\"liboktetagui\")

include_directories(
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

ecm_generate_headers(libcolumnsview_CCHDRS
  HEADER_NAMES
    KAdds # TODO: this header needs to be redone
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

set( oktetagui_LIB_HDRS
  ${libcolumnsview_HDRS}
  ${CMAKE_CURRENT_BINARY_DIR}/okteta/oktetagui_export.h
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

add_library( OktetaGui  SHARED ${oktetagui_LIB_SRCS} )

generate_export_header( OktetaGui EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/okteta/oktetagui_export.h)

target_link_libraries( OktetaGui
PUBLIC
  OktetaCore
  Qt5::Widgets
PRIVATE
  KF5::I18n
  KF5::ConfigWidgets
)

target_include_directories(OktetaGui PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

set_target_properties( OktetaGui  PROPERTIES
  EXPORT_NAME "OktetaGui"
  OUTPUT_NAME okteta${OKTETALIBS_ABI_VERSION}gui
  VERSION     ${OKTETAGUI_VERSION}
  SOVERSION   ${OKTETAGUI_SOVERSION}
)

install( TARGETS OktetaGui  EXPORT OktetaGuiTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${oktetagui_LIB_HDRS}
  DESTINATION ${OKTETALIBS_INCLUDE_INSTALL_DIR}
  COMPONENT Devel
)
install( FILES ${oktetagui_LIB_CCHDRS}
  DESTINATION ${OKTETALIBS_CCINCLUDE_INSTALL_DIR}
  COMPONENT Devel
)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/OktetaGui")

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/OktetaGuiConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/OktetaGuiConfig.cmake"
  INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install( FILES
    "${CMAKE_CURRENT_BINARY_DIR}/OktetaGuiConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/OktetaGuiConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT OktetaGuiTargets
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  FILE OktetaGuiTargets.cmake
)


if(BUILD_TESTING)
  add_subdirectory( test )
#   add_library( testoktetacore STATIC ${oktetagui_LIB_SRCS} )
endif()
