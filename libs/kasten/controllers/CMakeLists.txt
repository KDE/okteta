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
)

if (BUILD_TERMINALTOOL)
    target_link_libraries(KastenControllers PRIVATE KF5::Parts)
endif()

target_compile_definitions(KastenControllers PRIVATE -DTRANSLATION_DOMAIN=\"libkasten\")

macro(KASTENCONTROLLERS_ADD_SUBLIBRARY _subdir)
    okteta_add_sublibrary(Controllers  NAMESPACE Kasten
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

kastencontrollers_add_sublibrary(io/synchronize
    PUBLIC
        SynchronizeControllerFactory
    PRIVATE
        SynchronizeController
)

kastencontrollers_add_sublibrary(io/setremote
    PUBLIC
        SetRemoteControllerFactory
    PRIVATE
        SetRemoteController
)

kastencontrollers_add_sublibrary(documentsystem/creator
    PUBLIC
        CreatorControllerFactory
    PRIVATE
        CreatorController
)

kastencontrollers_add_sublibrary(documentsystem/loader
    PUBLIC
        LoaderControllerFactory
    PRIVATE
        LoaderController
)

kastencontrollers_add_sublibrary(documentsystem/close
    PUBLIC
        CloseControllerFactory
    PRIVATE
        CloseController
)

kastencontrollers_add_sublibrary(documentsystem/filesystembrowser
    PUBLIC
        FileSystemBrowserToolFactory
        FileSystemBrowserToolViewFactory
    PRIVATE
        FileSystemBrowserView
        FileSystemBrowserTool
        FileSystemBrowserToolView
)

kastencontrollers_add_sublibrary(documentsystem/documentsbrowser
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
kastencontrollers_add_sublibrary(document/terminal
    PUBLIC
        TerminalToolFactory
        TerminalToolViewFactory
    PRIVATE
        TerminalView
        TerminalTool
        TerminalToolView
)
endif()

kastencontrollers_add_sublibrary(document/modified
    PUBLIC
        ModifiedBarControllerFactory
    PRIVATE
        ModifiedBarController
        IconLabel
)

kastencontrollers_add_sublibrary(document/readonly
    PUBLIC
        ReadOnlyControllerFactory
        # TODO: two plugins in one subdir
        ReadOnlyBarControllerFactory
    PRIVATE
        ReadOnlyController
        ReadOnlyBarController
)

if(NOT CMAKE_BUILD_TYPE_TOLOWER MATCHES release)
kastencontrollers_add_sublibrary(document/versionview
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

kastencontrollers_add_sublibrary(view/version
    PUBLIC
        VersionControllerFactory
    PRIVATE
        VersionController
)

kastencontrollers_add_sublibrary(view/zoom
    PUBLIC
        ZoomControllerFactory
        # TODO: two plugins in one subdir
        ZoomBarControllerFactory
    PRIVATE
        ZoomSlider
        ZoomController
        ZoomBarController
)

kastencontrollers_add_sublibrary(view/select
    PUBLIC
        SelectControllerFactory
    PRIVATE
        SelectController
)

kastencontrollers_add_sublibrary(io/clipboard
    PUBLIC
        ClipboardControllerFactory
    PRIVATE
        ClipboardController
)

kastencontrollers_add_sublibrary(io/insert
    PUBLIC
        InsertControllerFactory
    PRIVATE
        InsertDialog
        InsertController
)

kastencontrollers_add_sublibrary(io/copyas
    PUBLIC
        CopyAsControllerFactory
    PRIVATE
        CopyAsDialog
        CopyAsController
)

kastencontrollers_add_sublibrary(io/export
    PUBLIC
        ExportControllerFactory
    PRIVATE
        ExportDialog
        ExportController
)

# set( CLOSECONTROLLER_SRCS
#   viewsystem/close/closecontroller.cpp
# )
kastencontrollers_add_sublibrary(shellwindow/viewareacontextmenu
    PUBLIC
        ViewAreaContextMenuControllerFactory
    PRIVATE
        ViewAreaContextMenuController
)

kastencontrollers_add_sublibrary(shellwindow/switchview
    PUBLIC
        SwitchViewControllerFactory
    PRIVATE
        SwitchViewController
)

kastencontrollers_add_sublibrary(shellwindow/viewlistmenu
    PUBLIC
        ViewListMenuControllerFactory
    PRIVATE
        ViewListMenuController
)

kastencontrollers_add_sublibrary(shellwindow/viewareasplit
    PUBLIC
        ViewAreaSplitControllerFactory
    PRIVATE
        ViewAreaSplitController
)

kastencontrollers_add_sublibrary(shellwindow/toollistmenu
    PUBLIC
        ToolListMenuControllerFactory
    PRIVATE
        ToolListMenuController
)

kastencontrollers_add_sublibrary(shellwindow/fullscreen
    PUBLIC
        FullScreenControllerFactory
    PRIVATE
        FullScreenController
)

kastencontrollers_add_sublibrary(program/uicolorscheme
    PUBLIC
        UiColorSchemeControllerFactory
    PRIVATE
        UiColorSchemeController
)

kastencontrollers_add_sublibrary(program/quit
    PUBLIC
        QuitControllerFactory
    PRIVATE
        QuitController
)

okteta_add_cmakeconfig(Controllers NAMESPACE Kasten
    DEPS
        KastenGui
)
