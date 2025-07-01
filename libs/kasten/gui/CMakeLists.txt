set(KASTENGUI_VERSION ${KASTEN_LIB_VERSION})
set(KASTENGUI_SO_VERSION ${KASTEN_LIB_SOVERSION})

okteta_add_library(Gui NAMESPACE Kasten
    PUBLIC
        KastenCore
        KF5::XmlGui
        Qt5::Widgets
    VERSION     ${KASTENGUI_VERSION}
    SOVERSION   ${KASTENGUI_SO_VERSION}
    ABIVERSION  ${KASTEN_ABI_VERSION}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_EXPORTED_TARGET_NAME
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
    KI18N_TRANSLATION_DOMAIN "libkasten"
)

macro(KASTENGUI_ADD_SUBLIBRARY _library_name)
    okteta_add_sublibrary(Gui NAMESPACE Kasten
        SUBLIBRARY ${_library_name}
        ${ARGN}
    )
endmacro()

macro(KASTENGUI_SUBLIBRARY_SOURCES _library_name)
    okteta_sublibrary_sources(Gui NAMESPACE Kasten
        SUBLIBRARY ${_library_name}
        ${ARGN}
    )
endmacro()

kastengui_add_sublibrary(KastenGuiView
    UNTESTED
    PUBLIC
        KastenCore
        Qt5::Widgets
)
okteta_qt_declare_logging_category(KastenGui
    HEADER "logging.hpp"
    IDENTIFIER "LOG_KASTEN_GUI"
    CATEGORY_NAME "kasten.gui"
    DESCRIPTION "Kasten Gui library"
    EXPORT OKTETA
)

kastengui_sublibrary_sources(KastenGuiView
    PUBLIC
        KastenGui
)

kastengui_sublibrary_sources(KastenGuiView
    SUBDIR view
    BUILD_INCLUDEDIR
    PUBLIC
        SelectedDataWriteable
        DataSelectable
        UserErrorReportsInlineable
        UserNotificationsInlineable
        Zoomable
        ZoomLevelsQueryable

        AbstractView
        AbstractViewFactory
        AbstractToolView
        AbstractToolViewFactory

        AbstractSelectionView
        AbstractToolInlineView
)

kastengui_add_sublibrary(KastenGuiIo
    UNTESTED
    PUBLIC
        KastenCore
        Qt5::Widgets
)

kastengui_sublibrary_sources(KastenGuiIo
    SUBDIR io
    BUILD_INCLUDEDIR
    PUBLIC
        AbstractModelStreamEncoderConfigEditor
        AbstractModelStreamEncoderConfigEditorFactory

        AbstractModelExporterConfigEditor
        AbstractModelExporterConfigEditorFactory

        AbstractModelDataGeneratorConfigEditor
        AbstractModelDataGeneratorConfigEditorFactory
)

kastengui_add_sublibrary(KastenGuiFileSystemIo
    UNTESTED
    PUBLIC
        KastenGuiIo
)

kastengui_sublibrary_sources(KastenGuiFileSystemIo
    SUBDIR io/filesystem
    PUBLIC
        ModelEncoderFileSystemExporterConfigEditor
        ModelEncoderFileSystemExporterConfigEditorFactory
)

kastengui_add_sublibrary(KastenGuiSystem
    UNTESTED
    PUBLIC
        KastenGuiView
        KastenGuiIo
    PRIVATE
        KF5::WidgetsAddons
        KF5::I18n
    KI18N_TRANSLATION_DOMAIN "libkasten"
)

kastengui_sublibrary_sources(KastenGuiSystem
    SUBDIR system
    BUILD_INCLUDEDIR
    PUBLIC
        AbstractDocumentStrategy
        MultiDocumentStrategy
        SingleDocumentStrategy

        DialogHandler
        UserMessagesHandler

        DummyView
        ModelCodecViewManager
        ViewManager

    PRIVATE
        CreateDialog
)

kastengui_add_sublibrary(KastenGuiController
    UNTESTED
    PUBLIC
        KastenCore
        Qt5::Widgets
)

kastengui_sublibrary_sources(KastenGuiController
    SUBDIR controller
    BUILD_INCLUDEDIR
    PUBLIC
        AbstractXmlGuiController
        AbstractXmlGuiControllerFactory

        AbstractLinePopup
        AbstractToolWidget
        ToggleButton
)

kastengui_add_sublibrary(KastenGuiShell
    UNTESTED
    PUBLIC
        KastenGuiView
        KastenGuiController
        KastenGuiSystem
        KF5::XmlGui
)

kastengui_sublibrary_sources(KastenGuiShell
    SUBDIR shell
    BUILD_INCLUDEDIR
    PUBLIC
        ToolInlineViewable
        WidgetsDockable
        ViewAreaSplitable

        AbstractViewArea
        AbstractGroupedViews

        SingleViewArea
        SingleViewWindow

        MultiViewAreas
        ShellWindow

        TabbedViews
        StatusBar
        ToolViewDockWidget

    PRIVATE
        StatusBarLayout
        TabBar
        TabWidget
        ToolInlineViewWidget
        ViewAreaBox
        ViewBox
)


okteta_add_cmakeconfig(Gui NAMESPACE Kasten
    DEPS
        Qt5Widgets ${Qt5_VERSION}
        KastenCore
)
