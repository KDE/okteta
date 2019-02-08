set(KASTENCONTROLLERS_VERSION ${KASTEN_LIB_VERSION})
set(KASTENCONTROLLERS_SO_VERSION ${KASTEN_LIB_SOVERSION})

add_definitions(-DTRANSLATION_DOMAIN=\"libkasten\")

macro(KASTENCONTROLLERS_ADD_SUBLIBRARY _subdir)
    okteta_add_sublibrary(Controllers  NAMESPACE Kasten
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

kastencontrollers_add_sublibrary(io/synchronize
    PUBLIC
        SynchronizeController
        SynchronizeControllerFactory
)

kastencontrollers_add_sublibrary(io/setremote
    PUBLIC
        SetRemoteController
        SetRemoteControllerFactory
)

kastencontrollers_add_sublibrary(documentsystem/creator
    PUBLIC
        CreatorController
        CreatorControllerFactory
)

kastencontrollers_add_sublibrary(documentsystem/loader
    PUBLIC
        LoaderController
        LoaderControllerFactory
)

kastencontrollers_add_sublibrary(documentsystem/close
    PUBLIC
        CloseController
        CloseControllerFactory
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

kastencontrollers_add_sublibrary(document/terminal
    PUBLIC
        TerminalToolFactory
        TerminalToolViewFactory
    PRIVATE
        TerminalView
        TerminalTool
        TerminalToolView
)

kastencontrollers_add_sublibrary(document/modified
    PUBLIC
        ModifiedBarController
        ModifiedBarControllerFactory
)

kastencontrollers_add_sublibrary(document/readonly
    PUBLIC
        ReadOnlyController
        ReadOnlyControllerFactory
        # TODO: two plugins in one subdir
        ReadOnlyBarController
        ReadOnlyBarControllerFactory
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
        VersionController
        VersionControllerFactory
)

kastencontrollers_add_sublibrary(view/zoom
    PUBLIC
        ZoomController
        ZoomControllerFactory
        # TODO: two plugins in one subdir
        ZoomBarController
        ZoomBarControllerFactory
    PRIVATE
        ZoomSlider
)

kastencontrollers_add_sublibrary(view/select
    PUBLIC
        SelectController
        SelectControllerFactory
)

kastencontrollers_add_sublibrary(io/clipboard
    PUBLIC
        ClipboardController
        ClipboardControllerFactory
)

kastencontrollers_add_sublibrary(io/insert
    PUBLIC
        InsertController
        InsertControllerFactory
    PRIVATE
        InsertDialog
)

kastencontrollers_add_sublibrary(io/copyas
    PUBLIC
        CopyAsController
        CopyAsControllerFactory
    PRIVATE
        CopyAsDialog
)

kastencontrollers_add_sublibrary(io/export
    PUBLIC
        ExportController
        ExportControllerFactory
    PRIVATE
        ExportDialog
)

# set( CLOSECONTROLLER_SRCS
#   viewsystem/close/closecontroller.cpp
# )
kastencontrollers_add_sublibrary(shellwindow/switchview
    PUBLIC
        SwitchViewController
        SwitchViewControllerFactory
)

kastencontrollers_add_sublibrary(shellwindow/viewlistmenu
    PUBLIC
        ViewListMenuController
        ViewListMenuControllerFactory
)

kastencontrollers_add_sublibrary(shellwindow/viewareasplit
    PUBLIC
        ViewAreaSplitController
        ViewAreaSplitControllerFactory
)

kastencontrollers_add_sublibrary(shellwindow/toollistmenu
    PUBLIC
        ToolListMenuController
        ToolListMenuControllerFactory
)

kastencontrollers_add_sublibrary(shellwindow/fullscreen
    PUBLIC
        FullScreenController
        FullScreenControllerFactory
)

kastencontrollers_add_sublibrary(program/quit
    PUBLIC
        QuitController
        QuitControllerFactory
)


okteta_add_library(Controllers NAMESPACE Kasten
    PUBLIC
        KastenGui
        KastenCore
    PRIVATE
        KF5::I18n
        KF5::ConfigWidgets
        KF5::XmlGui
        KF5::KIOFileWidgets
        KF5::Parts
    VERSION     ${KASTENCONTROLLERS_VERSION}
    SOVERSION   ${KASTENCONTROLLERS_SO_VERSION}
    ABIVERSION  ${KASTEN_ABI_VERSION}
    SOURCES   ${kastencontrollers_LIB_SRCS}
    HEADERS   ${kastencontrollers_LIB_HDRS}
    CCHEADERS ${kastencontrollers_LIB_CCHDRS}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAMESPACE
)

okteta_add_cmakeconfig(Controllers NAMESPACE Kasten
    NO_TARGET_NAMESPACE
    VERSION     ${KASTENCONTROLLERS_VERSION}
#     ABIVERSION ${KASTEN_ABI_VERSION}
)
