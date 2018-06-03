ecm_setup_version( ${KASTEN_LIB_VERSION}
    VARIABLE_PREFIX KASTENGUI
    SOVERSION ${KASTEN_LIB_SOVERSION}
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KastenGuiConfigVersion.cmake"
)

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

        # remove on next API change, accidentally here
        StatusBarLayout
        ToolInlineViewWidget
        ViewAreaBox
        ViewBox

    PRIVATE
        TabBar
        TabWidget
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
    SOVERSION   ${KASTENGUI_SOVERSION}
    ABIVERSION  ${KASTEN_ABI_VERSION}
    SOURCES   ${kastengui_LIB_SRCS}
    HEADERS   ${kastengui_LIB_HDRS}
    CCHEADERS ${kastengui_LIB_CCHDRS}
    INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    LOWERCASE_LIB
    EXPORT KastenGuiTargets
)

okteta_add_cmakeconfig(Gui NAMESPACE Kasten
    NO_TARGET_NAMESPACE
    CONFIG_TEMPLATE "${CMAKE_CURRENT_SOURCE_DIR}/KastenGuiConfig.cmake.in"
    EXPORT KastenGuiTargets
#     ABIVERSION ${KASTEN_ABI_VERSION}
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/KastenGuiConfigVersion.cmake"
)
