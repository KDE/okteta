set(KASTENGUI_VERSION ${KASTEN_LIB_VERSION})
set(KASTENGUI_SO_VERSION ${KASTEN_LIB_SOVERSION})

add_definitions(-DTRANSLATION_DOMAIN=\"libkasten\")

include_directories(
  view
  io
  system
  shell
  controller
)

macro(KASTENGUI_ADD_SUBLIBRARY _subdir)
    okteta_add_sublibrary(Gui  NAMESPACE Kasten
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

kastengui_add_sublibrary(view
    PUBLIC
        SelectedDataWriteable
        DataSelectable
        Zoomable

        AbstractView
        AbstractViewFactory
        AbstractToolView
        AbstractToolViewFactory

        AbstractSelectionView
        AbstractToolInlineView
)

kastengui_add_sublibrary(io/filesystem
    PUBLIC
        ModelEncoderFileSystemExporterConfigEditor
        ModelEncoderFileSystemExporterConfigEditorFactory
)

kastengui_add_sublibrary(io
    PUBLIC
        AbstractModelStreamEncoderConfigEditor
        AbstractModelStreamEncoderConfigEditorFactory

        AbstractModelExporterConfigEditor
        AbstractModelExporterConfigEditorFactory

        AbstractModelDataGeneratorConfigEditor
        AbstractModelDataGeneratorConfigEditorFactory
)

kastengui_add_sublibrary(system
    PUBLIC
        AbstractDocumentStrategy
        MultiDocumentStrategy
        SingleDocumentStrategy

        DialogHandler
        DummyView
        ModelCodecViewManager
        ViewManager

    PRIVATE
        CreateDialog
)

kastengui_add_sublibrary(shell
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

kastengui_add_sublibrary(controller
    PUBLIC
        AbstractController
        AbstractXmlGuiController

        AbstractLinePopup
        AbstractToolWidget
        ToggleButton
)

okteta_add_sublibrary(Gui  NAMESPACE Kasten
    PUBLIC
        KastenGui
)


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
    SOURCES   ${kastengui_LIB_SRCS}
    HEADERS   ${kastengui_LIB_HDRS}
    CCHEADERS ${kastengui_LIB_CCHDRS}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAMESPACE
)

okteta_add_cmakeconfig(Gui NAMESPACE Kasten
    NO_TARGET_NAMESPACE
    VERSION     ${KASTENGUI_VERSION}
#     ABIVERSION ${KASTEN_ABI_VERSION}
)
