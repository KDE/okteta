project( kakaocontrollers )

include_directories(
  ../core/entity
  ../core/document
  ../core/io
  ../core/io/filesystem
  ../core/system
  ../gui/view
  ../gui/system
  ../gui/controller
)


set( OFFERCONTROLLER_SRCS
  io/offer/offercontroller.cpp
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
set( CONNECTORCONTROLLER_SRCS
  documentsystem/connector/connectorcontroller.cpp
)
set( CLOSECONTROLLER_SRCS
  documentsystem/close/closecontroller.cpp
)
set( READONLYCONTROLLER_SRCS
  document/readonly/readonlycontroller.cpp
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
set( USERSTOOL_SRCS
  document/users/userlisttablemodel.cpp
  document/users/useritemdelegate.cpp
  document/users/userlistview.cpp
  document/users/userscontroltool.cpp
  document/users/userscontroltoolview.cpp
)
# set( CLOSECONTROLLER_SRCS
#   viewsystem/close/closecontroller.cpp
# )
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

set( KAKAO_CONTROLLERS_SRCS
  ${SETREMOTECONTROLLER_SRCS}
  ${CLOSECONTROLLER_SRCS}
  ${VERSIONCONTROLLER_SRCS}
  ${VERSIONVIEWCONTROLLER_SRCS}
  ${READONLYCONTROLLER_SRCS}
  ${VIEWLISTMENUCONTROLLER_SRCS}
  ${TOOLLISTMENUCONTROLLER_SRCS}
  ${FULLSCREENCONTROLLER_SRCS}
  ${ZOOMCONTROLLER_SRCS}
  ${OFFERCONTROLLER_SRCS}
  ${CREATORCONTROLLER_SRCS}
  ${LOADERCONTROLLER_SRCS}
  ${CONNECTORCONTROLLER_SRCS}
  ${SYNCHRONIZECONTROLLER_SRCS}
  ${USERSTOOL_SRCS}
  ${SELECTCONTROLLER_SRCS}
  ${CLIPBOARDCONTROLLER_SRCS}
  ${COPYASCONTROLLER_SRCS}
  ${EXPORTCONTROLLER_SRCS}
  ${QUITCONTROLLER_SRCS}
)

kde4_add_library( kakaocontrollers STATIC ${KAKAO_CONTROLLERS_SRCS} )
