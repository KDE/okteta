project( kakaocontrollers )

include_directories(
  ../core/document
  ../core/io
  ../core/system
  ../gui/view
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
set( READONLYCONTROLLER_SRCS
  document/readonly/readonlycontroller.cpp
)
set( VERSIONVIEWCONTROLLER_SRCS
  document/versionview/versiontablemodel.cpp
  document/versionview/versionview.cpp
  document/versionview/versionviewcontroller.cpp
)
set( VERSIONCONTROLLER_SRCS
  view/version/versioncontroller.cpp
)
set( ZOOMCONTROLLER_SRCS
  view/zoom/zoomcontroller.cpp
)
set( SELECTCONTROLLER_SRCS
  view/select/selectcontroller.cpp
)
set( CLIPBOARDCONTROLLER_SRCS
  io/clipboard/clipboardcontroller.cpp
)
set( COPYASCONTROLLER_SRCS
  io/copyas/copyascontroller.cpp
)
set( EXPORTCONTROLLER_SRCS
  io/export/exportcontroller.cpp
)
# set( CLOSECONTROLLER_SRCS
#   viewsystem/close/closecontroller.cpp
# )
set( VIEWLISTMENUCONTROLLER_SRCS
  shellwindow/viewlistmenu/viewlistmenucontroller.cpp
)
set( FULLSCREENCONTROLLER_SRCS
  shellwindow/fullscreen/fullscreencontroller.cpp
)
set( QUITCONTROLLER_SRCS
  program/quit/quitcontroller.cpp
)

set( KAKAO_CONTROLLERS_SRCS
  ${SETREMOTECONTROLLER_SRCS}
  ${CLOSECONTROLLER_SRCS}
  ${VERSIONCONTROLLER_SRCS}
  ${VERSIONVIEWCONTROLLER_SRCS}
  ${READONLYCONTROLLER_SRCS}
  ${VIEWLISTMENUCONTROLLER_SRCS}
  ${FULLSCREENCONTROLLER_SRCS}
  ${ZOOMCONTROLLER_SRCS}
  ${CREATORCONTROLLER_SRCS}
  ${LOADERCONTROLLER_SRCS}
  ${SYNCHRONIZECONTROLLER_SRCS}
  ${SELECTCONTROLLER_SRCS}
  ${CLIPBOARDCONTROLLER_SRCS}
  ${COPYASCONTROLLER_SRCS}
  ${EXPORTCONTROLLER_SRCS}
  ${QUITCONTROLLER_SRCS}
)

kde4_add_library( kakaocontrollers STATIC ${KAKAO_CONTROLLERS_SRCS} )
