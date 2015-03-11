project( kastencontrollers )

ecm_setup_version( ${KASTEN_LIB_VERSION}
    VARIABLE_PREFIX KASTENCONTROLLERS
    SOVERSION ${KASTEN_LIB_SOVERSION}
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KastenControllersConfigVersion.cmake"
)

add_definitions(-DTRANSLATION_DOMAIN=\"libkasten\")

macro(KASTENCONTROLLERS_GENERATE_HEADERS _SUBLIB _RELATIVE)
    ecm_generate_headers("${_SUBLIB}_CCHDRS"
        HEADER_NAMES ${ARGN}
        RELATIVE ${_RELATIVE}
        PREFIX Kasten
        REQUIRED_HEADERS "${_SUBLIB}_HDRS"
    )
endmacro()

set( SYNCHRONIZECONTROLLER_SRCS
  io/synchronize/synchronizecontroller.cpp
)
kastencontrollers_generate_headers(SYNCHRONIZECONTROLLER io/synchronize
  SynchronizeController
)

set( SETREMOTECONTROLLER_SRCS
  io/setremote/setremotecontroller.cpp
)
kastencontrollers_generate_headers(SETREMOTECONTROLLER io/setremote
  SetRemoteController
)

set( CREATORCONTROLLER_SRCS
  documentsystem/creator/creatorcontroller.cpp
)
kastencontrollers_generate_headers(CREATORCONTROLLER documentsystem/creator
  CreatorController
)

set( LOADERCONTROLLER_SRCS
  documentsystem/loader/loadercontroller.cpp
)
kastencontrollers_generate_headers(LOADERCONTROLLER documentsystem/loader
  LoaderController
)

set( CLOSECONTROLLER_SRCS
  documentsystem/close/closecontroller.cpp
)
kastencontrollers_generate_headers(CLOSECONTROLLER documentsystem/close
  CloseController
)

set( FILESYSTEMBROWSERCONTROLLER_SRCS
  documentsystem/filesystembrowser/filesystembrowsertool.cpp
  documentsystem/filesystembrowser/filesystembrowserview.cpp
  documentsystem/filesystembrowser/filesystembrowsertoolview.cpp
)
kastencontrollers_generate_headers(FILESYSTEMBROWSERCONTROLLER documentsystem/filesystembrowser
  FileSystemBrowserTool
  FileSystemBrowserToolView
)

set( DOCUMENTSBROWSERCONTROLLER_SRCS
  documentsystem/documentsbrowser/documentlistmodel.cpp
  documentsystem/documentsbrowser/documentstool.cpp
  documentsystem/documentsbrowser/documentsview.cpp
  documentsystem/documentsbrowser/documentstoolview.cpp
)
kastencontrollers_generate_headers(DOCUMENTSBROWSERCONTROLLER documentsystem/documentsbrowser
  DocumentsTool
  DocumentsToolView
)

set( TERMINALCONTROLLER_SRCS
  document/terminal/terminaltool.cpp
  document/terminal/terminalview.cpp
  document/terminal/terminaltoolview.cpp
)
kastencontrollers_generate_headers(TERMINALCONTROLLER document/terminal
  TerminalTool
  TerminalToolView
)

set( MODIFIEDBARCONTROLLER_SRCS
  document/modified/modifiedbarcontroller.cpp
)
kastencontrollers_generate_headers(MODIFIEDBARCONTROLLER document/modified
  ModifiedBarController
)

set( READONLYCONTROLLER_SRCS
  document/readonly/readonlycontroller.cpp
)
kastencontrollers_generate_headers(READONLYCONTROLLER document/readonly
  ReadOnlyController
)

set( READONLYBARCONTROLLER_SRCS
  document/readonly/readonlybarcontroller.cpp
)
kastencontrollers_generate_headers(READONLYBARCONTROLLER document/readonly
  ReadOnlyBarController
)

if(CMAKE_BUILD_TYPE_TOLOWER MATCHES release)
set( VERSIONVIEWCONTROLLER_SRCS )
else(CMAKE_BUILD_TYPE_TOLOWER MATCHES release)
set( VERSIONVIEWCONTROLLER_SRCS
  document/versionview/versiontablemodel.cpp
  document/versionview/versionview.cpp
  document/versionview/versionviewtool.cpp
  document/versionview/versionviewtoolview.cpp
)
kastencontrollers_generate_headers(VERSIONVIEWCONTROLLER document/versionview
  VersionViewTool
  VersionViewToolView
)
endif(CMAKE_BUILD_TYPE_TOLOWER MATCHES release)
set( VERSIONCONTROLLER_SRCS
  view/version/versioncontroller.cpp
)
kastencontrollers_generate_headers(VERSIONCONTROLLER view/version
  VersionController
)

set( ZOOMCONTROLLER_SRCS
  view/zoom/zoomcontroller.cpp
)
kastencontrollers_generate_headers(ZOOMCONTROLLER view/zoom
  ZoomController
)

set( ZOOMBARCONTROLLER_SRCS
  view/zoom/zoomslider.cpp
  view/zoom/zoombarcontroller.cpp
)
kastencontrollers_generate_headers(ZOOMBARCONTROLLER view/zoom
  ZoomBarController
)

set( SELECTCONTROLLER_SRCS
  view/select/selectcontroller.cpp
)
kastencontrollers_generate_headers(SELECTCONTROLLER view/select
  SelectController
)

set( CLIPBOARDCONTROLLER_SRCS
  io/clipboard/clipboardcontroller.cpp
)
kastencontrollers_generate_headers(CLIPBOARDCONTROLLER io/clipboard
  ClipBoardController
)

set( INSERTCONTROLLER_SRCS
  io/insert/insertdialog.cpp
  io/insert/insertcontroller.cpp
)
kastencontrollers_generate_headers(INSERTCONTROLLER io/insert
  InsertController
)

set( COPYASCONTROLLER_SRCS
  io/copyas/copyasdialog.cpp
  io/copyas/copyascontroller.cpp
)
kastencontrollers_generate_headers(COPYASCONTROLLER io/copyas
  CopyAsController
)

set( EXPORTCONTROLLER_SRCS
  io/export/exportdialog.cpp
  io/export/exportcontroller.cpp
)
kastencontrollers_generate_headers(EXPORTCONTROLLER io/export
  ExportController
)

# set( CLOSECONTROLLER_SRCS
#   viewsystem/close/closecontroller.cpp
# )
set( SWITCHVIEWCONTROLLER_SRCS
  shellwindow/switchview/switchviewcontroller.cpp
)
kastencontrollers_generate_headers(SWITCHVIEWCONTROLLER shellwindow/switchview
  SwitchViewController
)

set( VIEWLISTMENUCONTROLLER_SRCS
  shellwindow/viewlistmenu/viewlistmenucontroller.cpp
)
kastencontrollers_generate_headers(VIEWLISTMENUCONTROLLER shellwindow/viewlistmenu
  ViewListMenuController
)

set( VIEWAREASPLITCONTROLLER_SRCS
  shellwindow/viewareasplit/viewareasplitcontroller.cpp
)
kastencontrollers_generate_headers(VIEWAREASPLITCONTROLLER shellwindow/viewareasplit
  ViewAreaSplitController
)

set( TOOLLISTMENUCONTROLLER_SRCS
  shellwindow/toollistmenu/toollistmenucontroller.cpp
)
kastencontrollers_generate_headers(TOOLLISTMENUCONTROLLER shellwindow/toollistmenu
  ToolListMenuController
)

set( FULLSCREENCONTROLLER_SRCS
  shellwindow/fullscreen/fullscreencontroller.cpp
)
kastencontrollers_generate_headers(FULLSCREENCONTROLLER shellwindow/fullscreen
  FullScreenController
)

set( QUITCONTROLLER_SRCS
  program/quit/quitcontroller.cpp
)
kastencontrollers_generate_headers(QUITCONTROLLER program/quit
  QuitController
)

set( kastencontrollers_LIB_SRCS
  ${SETREMOTECONTROLLER_SRCS}
  ${CLOSECONTROLLER_SRCS}
  ${FILESYSTEMBROWSERCONTROLLER_SRCS}
  ${DOCUMENTSBROWSERCONTROLLER_SRCS}
  ${TERMINALCONTROLLER_SRCS}
  ${VERSIONCONTROLLER_SRCS}
  ${VERSIONVIEWCONTROLLER_SRCS}
  ${MODIFIEDBARCONTROLLER_SRCS}
  ${READONLYCONTROLLER_SRCS}
  ${READONLYBARCONTROLLER_SRCS}
  ${SWITCHVIEWCONTROLLER_SRCS}
  ${VIEWLISTMENUCONTROLLER_SRCS}
  ${VIEWAREASPLITCONTROLLER_SRCS}
  ${TOOLLISTMENUCONTROLLER_SRCS}
  ${FULLSCREENCONTROLLER_SRCS}
  ${ZOOMCONTROLLER_SRCS}
  ${ZOOMBARCONTROLLER_SRCS}
  ${CREATORCONTROLLER_SRCS}
  ${LOADERCONTROLLER_SRCS}
  ${SYNCHRONIZECONTROLLER_SRCS}
  ${SELECTCONTROLLER_SRCS}
  ${CLIPBOARDCONTROLLER_SRCS}
  ${INSERTCONTROLLER_SRCS}
  ${COPYASCONTROLLER_SRCS}
  ${EXPORTCONTROLLER_SRCS}
  ${QUITCONTROLLER_SRCS}
)

set( kastencontrollers_LIB_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/kastencontrollers_export.h
  ${SETREMOTECONTROLLER_HDRS}
  ${CLOSECONTROLLER_HDRS}
  ${FILESYSTEMBROWSERCONTROLLER_HDRS}
  ${DOCUMENTSBROWSERCONTROLLER_HDRS}
  ${TERMINALCONTROLLER_HDRS}
  ${VERSIONCONTROLLER_HDRS}
  ${VERSIONVIEWCONTROLLER_HDRS}
  ${MODIFIEDBARCONTROLLER_HDRS}
  ${READONLYCONTROLLER_HDRS}
  ${READONLYBARCONTROLLER_HDRS}
  ${SWITCHVIEWCONTROLLER_HDRS}
  ${VIEWLISTMENUCONTROLLER_HDRS}
  ${VIEWAREASPLITCONTROLLER_HDRS}
  ${TOOLLISTMENUCONTROLLER_HDRS}
  ${FULLSCREENCONTROLLER_HDRS}
  ${ZOOMCONTROLLER_HDRS}
  ${ZOOMBARCONTROLLER_HDRS}
  ${CREATORCONTROLLER_HDRS}
  ${LOADERCONTROLLER_HDRS}
  ${SYNCHRONIZECONTROLLER_HDRS}
  ${SELECTCONTROLLER_HDRS}
  ${CLIPBOARDCONTROLLER_HDRS}
  ${INSERTCONTROLLER_HDRS}
  ${COPYASCONTROLLER_HDRS}
  ${EXPORTCONTROLLER_HDRS}
  ${QUITCONTROLLER_HDRS}
)

set( kastencontrollers_LIB_CCHDRS
  ${SETREMOTECONTROLLER_CCHDRS}
  ${CLOSECONTROLLER_CCHDRS}
  ${FILESYSTEMBROWSERCONTROLLER_CCHDRS}
  ${DOCUMENTSBROWSERCONTROLLER_CCHDRS}
  ${TERMINALCONTROLLER_CCHDRS}
  ${VERSIONCONTROLLER_CCHDRS}
  ${VERSIONVIEWCONTROLLER_CCHDRS}
  ${MODIFIEDBARCONTROLLER_CCHDRS}
  ${READONLYCONTROLLER_CCHDRS}
  ${READONLYBARCONTROLLER_CCHDRS}
  ${SWITCHVIEWCONTROLLER_CCHDRS}
  ${VIEWLISTMENUCONTROLLER_CCHDRS}
  ${VIEWAREASPLITCONTROLLER_CCHDRS}
  ${TOOLLISTMENUCONTROLLER_CCHDRS}
  ${FULLSCREENCONTROLLER_CCHDRS}
  ${ZOOMCONTROLLER_CCHDRS}
  ${ZOOMBARCONTROLLER_CCHDRS}
  ${CREATORCONTROLLER_CCHDRS}
  ${LOADERCONTROLLER_CCHDRS}
  ${SYNCHRONIZECONTROLLER_CCHDRS}
  ${SELECTCONTROLLER_CCHDRS}
  ${CLIPBOARDCONTROLLER_CCHDRS}
  ${INSERTCONTROLLER_CCHDRS}
  ${COPYASCONTROLLER_CCHDRS}
  ${EXPORTCONTROLLER_CCHDRS}
  ${QUITCONTROLLER_CCHDRS}
)

add_library( KastenControllers SHARED ${kastencontrollers_LIB_SRCS} )
generate_export_header( KastenControllers )
target_link_libraries( KastenControllers
PUBLIC
  KastenGui
  KastenCore
PRIVATE
  KF5::I18n
  KF5::ConfigWidgets
  KF5::XmlGui
  KF5::KIOFileWidgets
  KF5::Parts
)

target_include_directories(KastenControllers PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/kasten>")

set_target_properties( KastenControllers  PROPERTIES
  EXPORT_NAME "KastenControllers"
  OUTPUT_NAME kasten${KASTEN_ABI_VERSION}controllers
  VERSION     ${KASTENCONTROLLERS_VERSION}
  SOVERSION   ${KASTENCONTROLLERS_SOVERSION}
)

install( TARGETS KastenControllers  EXPORT KastenControllersTargets ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${kastencontrollers_LIB_HDRS}
  DESTINATION ${KASTEN_INCLUDE_INSTALL_DIR}
  COMPONENT   Devel
)
install( FILES ${kastencontrollers_LIB_CCHDRS}
  DESTINATION ${KASTEN_CCINCLUDE_INSTALL_DIR}
  COMPONENT Devel
)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/Kasten")

ecm_configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KastenControllersConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KastenControllersConfig.cmake"
  INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install( FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KastenControllersConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KastenControllersConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KastenControllersTargets
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  FILE KastenControllersTargets.cmake
)
