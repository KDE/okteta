set(KASTENGUI_VERSION ${KASTEN_LIB_VERSION})
set(KASTENGUI_SO_VERSION ${KASTEN_LIB_SOVERSION})

okteta_add_library(Gui NAMESPACE Kasten
    PUBLIC
        KastenCore
        Qt5::Widgets
        KF5::XmlGui
    PRIVATE
        KF5::I18n
    VERSION     ${KASTENGUI_VERSION}
    SOVERSION   ${KASTENGUI_SO_VERSION}
    ABIVERSION  ${KASTEN_ABI_VERSION}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_EXPORTED_TARGET_NAME
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
)

ecm_qt_declare_logging_category(KastenGui
    HEADER "logging.hpp"
    IDENTIFIER "LOG_KASTEN_GUI"
    CATEGORY_NAME "kasten.gui"
    DESCRIPTION "Kasten Gui library"
    EXPORT OKTETA
)

target_compile_definitions(KastenGui PRIVATE -DTRANSLATION_DOMAIN=\"libkasten\")

macro(KASTENGUI_LIBRARY_SOURCES _subdir)
    okteta_library_sources(Gui NAMESPACE Kasten
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

kastengui_library_sources(view
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

kastengui_library_sources(io/filesystem
    PUBLIC
        ModelEncoderFileSystemExporterConfigEditor
        ModelEncoderFileSystemExporterConfigEditorFactory
)

kastengui_library_sources(io
    BUILD_INCLUDEDIR
    PUBLIC
        AbstractModelStreamEncoderConfigEditor
        AbstractModelStreamEncoderConfigEditorFactory

        AbstractModelExporterConfigEditor
        AbstractModelExporterConfigEditorFactory

        AbstractModelDataGeneratorConfigEditor
        AbstractModelDataGeneratorConfigEditorFactory
)

kastengui_library_sources(system
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

kastengui_library_sources(shell
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

kastengui_library_sources(controller
    BUILD_INCLUDEDIR
    PUBLIC
        AbstractXmlGuiController
        AbstractXmlGuiControllerFactory

        AbstractLinePopup
        AbstractToolWidget
        ToggleButton
)

okteta_library_sources(Gui  NAMESPACE Kasten
    PUBLIC
        KastenGui
)

okteta_add_cmakeconfig(Gui NAMESPACE Kasten
    DEPS
        Qt5Widgets ${Qt5_VERSION}
        KastenCore
)
