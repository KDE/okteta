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
)

kastencontrollers_add_sublibrary(io/setremote
    PUBLIC
        SetRemoteController
)

kastencontrollers_add_sublibrary(documentsystem/creator
    PUBLIC
        CreatorController
)

kastencontrollers_add_sublibrary(documentsystem/loader
    PUBLIC
        LoaderController
)

kastencontrollers_add_sublibrary(documentsystem/close
    PUBLIC
        CloseController
)

kastencontrollers_add_sublibrary(documentsystem/filesystembrowser
    PUBLIC
        FileSystemBrowserTool
        FileSystemBrowserToolView
    PRIVATE
        FileSystemBrowserView
)

kastencontrollers_add_sublibrary(documentsystem/documentsbrowser
    PUBLIC
        DocumentsTool
        DocumentsToolView
    PRIVATE
        DocumentsView
        DocumentListModel
)

kastencontrollers_add_sublibrary(document/terminal
    PUBLIC
        TerminalTool
        TerminalToolView
    PRIVATE
        TerminalView
)

kastencontrollers_add_sublibrary(document/modified
    PUBLIC
        ModifiedBarController
)

kastencontrollers_add_sublibrary(document/readonly
    PUBLIC
        ReadOnlyController
        # TODO: two plugins in one subdir
        ReadOnlyBarController
)

if(NOT CMAKE_BUILD_TYPE_TOLOWER MATCHES release)
kastencontrollers_add_sublibrary(document/versionview
    PUBLIC
        VersionViewTool
        VersionViewToolView
    PRIVATE
        VersionTableModel
        VersionView
)
endif()

kastencontrollers_add_sublibrary(view/version
    PUBLIC
        VersionController
)

kastencontrollers_add_sublibrary(view/zoom
    PUBLIC
        ZoomController
        # TODO: two plugins in one subdir
        ZoomBarController
    PRIVATE
        ZoomSlider
)

kastencontrollers_add_sublibrary(view/select
    PUBLIC
        SelectController
)

kastencontrollers_add_sublibrary(io/clipboard
    PUBLIC
        ClipboardController
)

kastencontrollers_add_sublibrary(io/insert
    PUBLIC
        InsertController
    PRIVATE
        InsertDialog
)

kastencontrollers_add_sublibrary(io/copyas
    PUBLIC
        CopyAsController
    PRIVATE
        CopyAsDialog
)

kastencontrollers_add_sublibrary(io/export
    PUBLIC
        ExportController
    PRIVATE
        ExportDialog
)

# set( CLOSECONTROLLER_SRCS
#   viewsystem/close/closecontroller.cpp
# )
kastencontrollers_add_sublibrary(shellwindow/switchview
    PUBLIC
        SwitchViewController
)

kastencontrollers_add_sublibrary(shellwindow/viewlistmenu
    PUBLIC
        ViewListMenuController
)

kastencontrollers_add_sublibrary(shellwindow/viewareasplit
    PUBLIC
        ViewAreaSplitController
)

kastencontrollers_add_sublibrary(shellwindow/toollistmenu
    PUBLIC
        ToolListMenuController
)

kastencontrollers_add_sublibrary(shellwindow/fullscreen
    PUBLIC
        FullScreenController
)

kastencontrollers_add_sublibrary(program/quit
    PUBLIC
        QuitController
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
