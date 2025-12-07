set(KASTENCONTROLLERS_VERSION ${KASTEN_LIB_VERSION})
set(KASTENCONTROLLERS_SO_VERSION ${KASTEN_LIB_SOVERSION})

okteta_add_library(Controllers NAMESPACE Kasten
    PUBLIC
        KastenGui
        KastenCore
    PRIVATE
        KF6::I18n
        KF6::ColorScheme
        KF6::ConfigWidgets
        KF6::XmlGui
        KF6::KIOFileWidgets
        KF6::Parts
    VERSION     ${KASTENCONTROLLERS_VERSION}
    SOVERSION   ${KASTENCONTROLLERS_SO_VERSION}
    ABIVERSION  ${KASTEN_ABI_VERSION}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    KI18N_TRANSLATION_DOMAIN "libkasten"
)

macro(KASTENCONTROLLERS_SOURCES)
    okteta_library_sources(Controllers NAMESPACE Kasten
        ${ARGN}
    )
endmacro()

kastencontrollers_sources(
    SUBDIR io/synchronize
    PUBLIC
        SynchronizeControllerFactory
    PRIVATE
        SynchronizeController
)

kastencontrollers_sources(
    SUBDIR io/setremote
    PUBLIC
        SetRemoteControllerFactory
    PRIVATE
        SetRemoteController
)

kastencontrollers_sources(
    SUBDIR documentsystem/creator
    PUBLIC
        CreatorControllerFactory
    PRIVATE
        CreatorController
)

kastencontrollers_sources(
    SUBDIR documentsystem/loader
    PUBLIC
        LoaderControllerFactory
    PRIVATE
        LoaderController
)

kastencontrollers_sources(
    SUBDIR documentsystem/close
    PUBLIC
        CloseControllerFactory
    PRIVATE
        CloseController
)

kastencontrollers_sources(
    SUBDIR documentsystem/filesystembrowser
    PUBLIC
        FileSystemBrowserToolFactory
        FileSystemBrowserToolViewFactory
    PRIVATE
        FileSystemBrowserView
        FileSystemBrowserTool
        FileSystemBrowserToolView
)

kastencontrollers_sources(
    SUBDIR documentsystem/documentsbrowser
    PUBLIC
        DocumentsToolFactory
        DocumentsToolViewFactory
    PRIVATE
        DocumentsView
        DocumentListModel
        DocumentsTool
        DocumentsToolView
)

kastencontrollers_sources(
    SUBDIR document/terminal
    PUBLIC
        TerminalToolFactory
        TerminalToolViewFactory
    PRIVATE
        TerminalView
        TerminalTool
        TerminalToolView
)

kastencontrollers_sources(
    SUBDIR document/modified
    PUBLIC
        ModifiedBarControllerFactory
    PRIVATE
        ModifiedBarController
        IconLabel
)

kastencontrollers_sources(
    SUBDIR document/readonly
    PUBLIC
        ReadOnlyControllerFactory
        # TODO: two plugins in one subdir
        ReadOnlyBarControllerFactory
    PRIVATE
        ReadOnlyController
        ReadOnlyBarController
)

if(NOT CMAKE_BUILD_TYPE_TOLOWER MATCHES release)
kastencontrollers_sources(
    SUBDIR document/versionview
    PUBLIC
        VersionViewToolFactory
        VersionViewToolViewFactory
    PRIVATE
        VersionTableModel
        VersionView
        VersionViewTool
        VersionViewToolView
)
endif()

kastencontrollers_sources(
    SUBDIR view/version
    PUBLIC
        VersionControllerFactory
    PRIVATE
        VersionController
)

kastencontrollers_sources(
    SUBDIR view/zoom
    PUBLIC
        ZoomControllerFactory
        # TODO: two plugins in one subdir
        ZoomBarControllerFactory
    PRIVATE
        ZoomSlider
        ZoomController
        ZoomBarController
)

kastencontrollers_sources(
    SUBDIR view/select
    PUBLIC
        SelectControllerFactory
    PRIVATE
        SelectController
)

kastencontrollers_sources(
    SUBDIR io/clipboard
    PUBLIC
        ClipboardControllerFactory
    PRIVATE
        ClipboardController
)

kastencontrollers_sources(
    SUBDIR io/insert
    PUBLIC
        InsertControllerFactory
    PRIVATE
        InsertDialog
        InsertController
)

kastencontrollers_sources(
    SUBDIR io/copyas
    PUBLIC
        CopyAsControllerFactory
    PRIVATE
        CopyAsDialog
        CopyAsController
)

kastencontrollers_sources(
    SUBDIR io/export
    PUBLIC
        ExportControllerFactory
    PRIVATE
        ExportDialog
        ExportController
)

# set( CLOSECONTROLLER_SRCS
#   viewsystem/close/closecontroller.cpp
# )
kastencontrollers_sources(
    SUBDIR shellwindow/viewareacontextmenu
    PUBLIC
        ViewAreaContextMenuControllerFactory
    PRIVATE
        ViewAreaContextMenuController
)

kastencontrollers_sources(
    SUBDIR shellwindow/switchview
    PUBLIC
        SwitchViewControllerFactory
    PRIVATE
        SwitchViewController
)

kastencontrollers_sources(
    SUBDIR shellwindow/viewlistmenu
    PUBLIC
        ViewListMenuControllerFactory
    PRIVATE
        ViewListMenuController
)

kastencontrollers_sources(
    SUBDIR shellwindow/viewareasplit
    PUBLIC
        ViewAreaSplitControllerFactory
    PRIVATE
        ViewAreaSplitController
)

kastencontrollers_sources(
    SUBDIR shellwindow/toollistmenu
    PUBLIC
        ToolListMenuControllerFactory
    PRIVATE
        ToolListMenuController
)

kastencontrollers_sources(
    SUBDIR shellwindow/fullscreen
    PUBLIC
        FullScreenControllerFactory
    PRIVATE
        FullScreenController
)

kastencontrollers_sources(
    SUBDIR program/uicolorscheme
    PUBLIC
        UiColorSchemeControllerFactory
    PRIVATE
        UiColorSchemeController
)

kastencontrollers_sources(
    SUBDIR program/quit
    PUBLIC
        QuitControllerFactory
    PRIVATE
        QuitController
)

okteta_add_cmakeconfig(Controllers NAMESPACE Kasten
    DEPS
        KastenGui
)
