ecm_setup_version( ${OKTETALIBS_LIB_VERSION}
    VARIABLE_PREFIX OKTETAGUI
    SOVERSION ${OKTETALIBS_LIB_SOVERSION}
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/OktetaGuiConfigVersion.cmake"
)

add_definitions(-DTRANSLATION_DOMAIN=\"liboktetagui\")

include_directories(
    libcolumnsview
)

macro(OKTETAGUI_ADD_SUBLIBRARY _subdir)
    okteta_add_sublibrary(Gui NAMESPACE Okteta
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

oktetagui_add_sublibrary(controller
    PRIVATE
        KController
        AbstractMouseController
        AbstractWheelController
        KEditor
        KNavigator
        KTabController
        KCharEditor
        KValueEditor
        MouseNavigator
        MousePaster
        # Dragger
        ZoomWheelController
        Dropper
)

oktetagui_add_sublibrary(libcolumnsview
    PUBLIC
        KAdds # TODO: this header needs to be redone
        AbstractColumnStylist
        AbstractColumnRenderer
        ColumnsView

    PRIVATE
        AbstractColumnStylist
        AbstractColumnRenderer
)

okteta_add_sublibrary(Gui  NAMESPACE Okteta
    PUBLIC
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

    PRIVATE
        ByteArrayTableCursor
        KCursor
)


okteta_add_library(Gui NAMESPACE Okteta
    PUBLIC
        OktetaCore
        Qt5::Widgets
    PRIVATE
        KF5::I18n
        KF5::ConfigWidgets
    VERSION     ${OKTETAGUI_VERSION}
    SOVERSION   ${OKTETAGUI_SOVERSION}
    ABIVERSION  ${OKTETALIBS_ABI_VERSION}
    SOURCES   ${oktetagui_LIB_SRCS}
    HEADERS   ${oktetagui_LIB_HDRS}
    CCHEADERS ${oktetagui_LIB_CCHDRS}
    INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    LOWERCASE_LIB
    EXPORT OktetaGuiTargets
)

okteta_add_cmakeconfig(Gui NAMESPACE Okteta
    NO_TARGET_NAMESPACE
    CONFIG_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/OktetaGuiConfig.cmake.in"
    EXPORT OktetaGuiTargets
#     ABIVERSION ${OKTETALIBS_ABI_VERSION}
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/OktetaGuiConfigVersion.cmake"
)

if(BUILD_TESTING)
  add_subdirectory( test )
#   add_library( testoktetacore STATIC ${oktetagui_LIB_SRCS} )
endif()
