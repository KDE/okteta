set(KASTENCONTROLLERS_VERSION ${KASTEN_LIB_VERSION})
set(KASTENCONTROLLERS_SO_VERSION ${KASTEN_LIB_SOVERSION})

okteta_add_library(Controllers NAMESPACE Kasten
    PUBLIC
        KastenGui
        KastenCore
    PRIVATE
        KF5::I18n
        KF5::ConfigWidgets
        KF5::XmlGui
        KF5::KIOFileWidgets
    VERSION     ${KASTENCONTROLLERS_VERSION}
    SOVERSION   ${KASTENCONTROLLERS_SO_VERSION}
    ABIVERSION  ${KASTEN_ABI_VERSION}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_EXPORTED_TARGET_NAME
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
    KI18N_TRANSLATION_DOMAIN "libkasten"
)

if (BUILD_TERMINALTOOL)
    target_link_libraries(KastenControllers PRIVATE KF5::Parts)
endif()

macro(KASTENCONTROLLERS_LIBRARY_SOURCES _subdir)
    okteta_library_sources(Controllers NAMESPACE Kasten
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

kastencontrollers_library_sources(io/synchronize
    PUBLIC
        SynchronizeControllerFactory
    PRIVATE
        SynchronizeController
)

kastencontrollers_library_sources(io/setremote
    PUBLIC
        SetRemoteControllerFactory
    PRIVATE
        SetRemoteController
)

kastencontrollers_library_sources(documentsystem/creator
    PUBLIC
        CreatorControllerFactory
    PRIVATE
        CreatorController
)

kastencontrollers_library_sources(documentsystem/loader
    PUBLIC
        LoaderControllerFactory
    PRIVATE
        LoaderController
)

kastencontrollers_library_sources(documentsystem/close
    PUBLIC
        CloseControllerFactory
    PRIVATE
        CloseController
)

kastencontrollers_library_sources(documentsystem/filesystembrowser
    PUBLIC
        FileSystemBrowserToolFactory
        FileSystemBrowserToolViewFactory
    PRIVATE
        FileSystemBrowserView
        FileSystemBrowserTool
        FileSystemBrowserToolView
)

kastencontrollers_library_sources(documentsystem/documentsbrowser
    PUBLIC
        DocumentsToolFactory
        DocumentsToolViewFactory
    PRIVATE
        DocumentsView
        DocumentListModel
        DocumentsTool
        DocumentsToolView
)

if (BUILD_TERMINALTOOL)
kastencontrollers_library_sources(document/terminal
    PUBLIC
        TerminalToolFactory
        TerminalToolViewFactory
    PRIVATE
        TerminalView
        TerminalTool
        TerminalToolView
)
endif()

kastencontrollers_library_sources(document/modified
    PUBLIC
        ModifiedBarControllerFactory
    PRIVATE
        ModifiedBarController
        IconLabel
)

kastencontrollers_library_sources(document/readonly
    PUBLIC
        ReadOnlyControllerFactory
        # TODO: two plugins in one subdir
        ReadOnlyBarControllerFactory
    PRIVATE
        ReadOnlyController
        ReadOnlyBarController
)

if(NOT CMAKE_BUILD_TYPE_TOLOWER MATCHES release)
kastencontrollers_library_sources(document/versionview
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

kastencontrollers_library_sources(view/version
    PUBLIC
        VersionControllerFactory
    PRIVATE
        VersionController
)

kastencontrollers_library_sources(view/zoom
    PUBLIC
        ZoomControllerFactory
        # TODO: two plugins in one subdir
        ZoomBarControllerFactory
    PRIVATE
        ZoomSlider
        ZoomController
        ZoomBarController
)

kastencontrollers_library_sources(view/select
    PUBLIC
        SelectControllerFactory
    PRIVATE
        SelectController
)

kastencontrollers_library_sources(io/clipboard
    PUBLIC
        ClipboardControllerFactory
    PRIVATE
        ClipboardController
)

kastencontrollers_library_sources(io/insert
    PUBLIC
        InsertControllerFactory
    PRIVATE
        InsertDialog
        InsertController
)

kastencontrollers_library_sources(io/copyas
    PUBLIC
        CopyAsControllerFactory
    PRIVATE
        CopyAsDialog
        CopyAsController
)

kastencontrollers_library_sources(io/export
    PUBLIC
        ExportControllerFactory
    PRIVATE
        ExportDialog
        ExportController
)

# set( CLOSECONTROLLER_SRCS
#   viewsystem/close/closecontroller.cpp
# )
kastencontrollers_library_sources(shellwindow/viewareacontextmenu
    PUBLIC
        ViewAreaContextMenuControllerFactory
    PRIVATE
        ViewAreaContextMenuController
)

kastencontrollers_library_sources(shellwindow/switchview
    PUBLIC
        SwitchViewControllerFactory
    PRIVATE
        SwitchViewController
)

kastencontrollers_library_sources(shellwindow/viewlistmenu
    PUBLIC
        ViewListMenuControllerFactory
    PRIVATE
        ViewListMenuController
)

kastencontrollers_library_sources(shellwindow/viewareasplit
    PUBLIC
        ViewAreaSplitControllerFactory
    PRIVATE
        ViewAreaSplitController
)

kastencontrollers_library_sources(shellwindow/toollistmenu
    PUBLIC
        ToolListMenuControllerFactory
    PRIVATE
        ToolListMenuController
)

kastencontrollers_library_sources(shellwindow/fullscreen
    PUBLIC
        FullScreenControllerFactory
    PRIVATE
        FullScreenController
)

kastencontrollers_library_sources(program/uicolorscheme
    PUBLIC
        UiColorSchemeControllerFactory
    PRIVATE
        UiColorSchemeController
)

kastencontrollers_library_sources(program/quit
    PUBLIC
        QuitControllerFactory
    PRIVATE
        QuitController
)

okteta_add_cmakeconfig(Controllers NAMESPACE Kasten
    DEPS
        KastenGui
)
