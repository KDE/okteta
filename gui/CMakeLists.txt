set(OKTETAGUI_VERSION ${OKTETALIBS_LIB_VERSION})
set(OKTETAGUI_SO_VERSION ${OKTETALIBS_LIB_SOVERSION})

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
    NO_TARGET_NAMESPACE
    NO_VERSIONED_EXPORTED_TARGET_NAME
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
    KI18N_TRANSLATION_DOMAIN "liboktetagui"
)

macro(OKTETAGUI_SOURCES)
    okteta_library_sources(Gui NAMESPACE Okteta
        ${ARGN}
    )
endmacro()

ecm_qt_declare_logging_category(OktetaGui
    HEADER "logging.hpp"
    IDENTIFIER "LOG_OKTETA_GUI"
    CATEGORY_NAME "okteta.gui"
    DESCRIPTION "Okteta Gui library"
    EXPORT OKTETA
)

okteta_add_sublibrary(Gui NAMESPACE Okteta
    SUBLIBRARY OktetaGuiColumnsView
    UNTESTED
    PUBLIC
        OktetaCore
        Qt5::Widgets
    PRIVATE
        KF5::I18n
    KI18N_TRANSLATION_DOMAIN "liboktetagui"
)

okteta_sublibrary_sources(Gui NAMESPACE Okteta
    SUBLIBRARY OktetaGuiColumnsView
    SUBDIR libcolumnsview
    BUILD_INCLUDEDIR
    PUBLIC
        AbstractColumnStylist
        AbstractColumnRenderer
        PixelMetrics # TODO: this header needs to be redone

    PRIVATE
        ColumnsViewScrollAreaEngine
)

oktetagui_sources(
    SUBDIR controller
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

oktetagui_sources(
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

okteta_add_cmakeconfig(Gui NAMESPACE Okteta
    DEPS
        OktetaCore
        Qt5Widgets ${Qt5_VERSION}
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
