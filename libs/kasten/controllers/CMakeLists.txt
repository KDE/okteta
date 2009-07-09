project( kastencontrollers )

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=1805 )

include_directories(
  ../core/entity
  ../core/document
  ../core/io
  ../core/io/filesystem
  ../core/system
  ../gui/view
  ../gui/io
  ../gui/system
  ../gui/controller
)


set( SYNCHRONIZECONTROLLER_SRCS
  io/synchronize/synchronizecontroller.cpp
)
set( SETREMOTECONTROLLER_SRCS
  io/setremote/setremotecontroller.cpp
)
set( CREATORCONTROLLER_SRCS
  documentsystem/creator/creatorcontroller.cpp
)
set( LOADERCONTROLLER_SRCS
  documentsystem/loader/loadercontroller.cpp
)
set( CLOSECONTROLLER_SRCS
  documentsystem/close/closecontroller.cpp
)
set( FILESYSTEMBROWSERCONTROLLER_SRCS
  documentsystem/filesystembrowser/filesystembrowsertool.cpp
  documentsystem/filesystembrowser/filesystembrowserview.cpp
  documentsystem/filesystembrowser/filesystembrowsertoolview.cpp
)
set( DOCUMENTSBROWSERCONTROLLER_SRCS
  documentsystem/documentsbrowser/documentlistmodel.cpp
  documentsystem/documentsbrowser/documentstool.cpp
  documentsystem/documentsbrowser/documentsview.cpp
  documentsystem/documentsbrowser/documentstoolview.cpp
)
set( READONLYCONTROLLER_SRCS
  document/readonly/readonlycontroller.cpp
)
set( READONLYBARCONTROLLER_SRCS
  document/readonly/readonlybarcontroller.cpp
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
endif(CMAKE_BUILD_TYPE_TOLOWER MATCHES release)
set( VERSIONCONTROLLER_SRCS
  view/version/versioncontroller.cpp
)
set( ZOOMCONTROLLER_SRCS
  view/zoom/zoomcontroller.cpp
)
set( ZOOMBARCONTROLLER_SRCS
  view/zoom/zoomslider.cpp
  view/zoom/zoombarcontroller.cpp
)
set( SELECTCONTROLLER_SRCS
  view/select/selectcontroller.cpp
)
set( CLIPBOARDCONTROLLER_SRCS
  io/clipboard/clipboardcontroller.cpp
)
set( COPYASCONTROLLER_SRCS
  io/copyas/copyasdialog.cpp
  io/copyas/copyascontroller.cpp
)
set( EXPORTCONTROLLER_SRCS
  io/export/exportdialog.cpp
  io/export/exportcontroller.cpp
)
# set( CLOSECONTROLLER_SRCS
#   viewsystem/close/closecontroller.cpp
# )
set( SWITCHVIEWCONTROLLER_SRCS
  shellwindow/switchview/switchviewcontroller.cpp
)
set( VIEWLISTMENUCONTROLLER_SRCS
  shellwindow/viewlistmenu/viewlistmenucontroller.cpp
)
set( TOOLLISTMENUCONTROLLER_SRCS
  shellwindow/toollistmenu/toollistmenucontroller.cpp
)
set( FULLSCREENCONTROLLER_SRCS
  shellwindow/fullscreen/fullscreencontroller.cpp
)
set( QUITCONTROLLER_SRCS
  program/quit/quitcontroller.cpp
)

set( KASTEN_CONTROLLERS_SRCS
  ${SETREMOTECONTROLLER_SRCS}
  ${CLOSECONTROLLER_SRCS}
  ${FILESYSTEMBROWSERCONTROLLER_SRCS}
  ${DOCUMENTSBROWSERCONTROLLER_SRCS}
  ${VERSIONCONTROLLER_SRCS}
  ${VERSIONVIEWCONTROLLER_SRCS}
  ${READONLYCONTROLLER_SRCS}
  ${READONLYBARCONTROLLER_SRCS}
  ${SWITCHVIEWCONTROLLER_SRCS}
  ${VIEWLISTMENUCONTROLLER_SRCS}
  ${TOOLLISTMENUCONTROLLER_SRCS}
  ${FULLSCREENCONTROLLER_SRCS}
  ${ZOOMCONTROLLER_SRCS}
  ${ZOOMBARCONTROLLER_SRCS}
  ${CREATORCONTROLLER_SRCS}
  ${LOADERCONTROLLER_SRCS}
  ${SYNCHRONIZECONTROLLER_SRCS}
  ${SELECTCONTROLLER_SRCS}
  ${CLIPBOARDCONTROLLER_SRCS}
  ${COPYASCONTROLLER_SRCS}
  ${EXPORTCONTROLLER_SRCS}
  ${QUITCONTROLLER_SRCS}
)

kde4_add_library( kastencontrollers STATIC ${KASTEN_CONTROLLERS_SRCS} )
