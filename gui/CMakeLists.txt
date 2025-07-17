set(OKTETAGUI_VERSION ${OKTETALIBS_LIB_VERSION})
set(OKTETAGUI_SO_VERSION ${OKTETALIBS_LIB_SOVERSION})

set(LIBOKTETAWIDGETS_REL_DIR ../kasten/gui/liboktetawidgets)

add_definitions(-DTRANSLATION_DOMAIN=\"liboktetagui\")

macro(OKTETAGUI_ADD_SUBLIBRARY _subdir)
    okteta_add_sublibrary(Gui NAMESPACE Okteta
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

oktetagui_add_sublibrary(controller
    PRIVATE
        AbstractController
        AbstractMouseController
        AbstractWheelController
        AbstractEditor
        KeyNavigator
        TabController
        ClipboardController
        UndoRedoController
        CharEditor
        ValueEditor
        MouseNavigator
        MousePaster
        TapNavigator
        # Dragger
        ZoomWheelController
        ZoomPinchController
        Dropper

        TouchOnlyTapAndHoldGesture
        TouchOnlyTapAndHoldGestureRecognizer
)

oktetagui_add_sublibrary(libcolumnsview
    PUBLIC
        AbstractColumnStylist
        AbstractColumnRenderer
        ColumnsView
        PixelMetrics # TODO: this header needs to be redone
)

# TODO: move to oktetacore, no longer duplicate
oktetagui_add_sublibrary(${LIBOKTETAWIDGETS_REL_DIR}
    PRIVATE
        ByteArrayChar8StringDecoder
        ByteArrayValueStringDecoder
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
        Cursor
        FontScalingZoomState
)


okteta_add_library(Gui NAMESPACE Okteta
    PUBLIC
        OktetaCore
        Qt5::Widgets
    PRIVATE
        KF5::I18n
        KF5::ConfigWidgets
    VERSION     ${OKTETAGUI_VERSION}
    SOVERSION   ${OKTETAGUI_SO_VERSION}
    ABIVERSION  ${OKTETALIBS_ABI_VERSION}
    SOURCES   ${oktetagui_LIB_SRCS}
    HEADERS   ${oktetagui_LIB_HDRS}
    CCHEADERS ${oktetagui_LIB_CCHDRS}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
)

target_include_directories(OktetaGui
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libcolumnsview>
    PRIVATE
        ${LIBOKTETAWIDGETS_REL_DIR}
)

okteta_add_cmakeconfig(Gui NAMESPACE Okteta
)

okteta_add_qmakeconfig(Gui NAMESPACE Okteta
    DEPS OktetaCore widgets
)

okteta_add_pkgconfig(Gui NAMESPACE Okteta
    DEPS OktetaCore Qt5Widgets
    DESCRIPTION "Okteta Gui"
)

if(BUILD_TESTING)
    add_subdirectory(tests)
#   add_library( testoktetacore STATIC ${oktetagui_LIB_SRCS} )
endif()
