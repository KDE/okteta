project( mobilekastencontrollers )

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=1805 )

set( KASTEN_REL_DIR ../../../libs/kasten )

include_directories(
  ${KASTEN_REL_DIR}/core
  ${KASTEN_REL_DIR}/core/document
  ${KASTEN_REL_DIR}/core/io
  ${KASTEN_REL_DIR}/core/system
  ${KASTEN_REL_DIR}/gui
  ${KASTEN_REL_DIR}/gui/controller
#   ../core
#   ../core/entity
#   ../core/document
#   ../core/io
#   ../core/io/filesystem
#   ../core/system
  ../gui
#   ../gui/view
#   ../gui/io
#   ../gui/system
  ../gui/shell
  ../gui/controller
)


set( synchronizecontroller_SRCS
#   io/synchronize/synchronizecontroller.cpp
)
set( setremotecontroller_SRCS
#   io/setremote/setremotecontroller.cpp
)
set( creatorcontroller_SRCS
#   documentsystem/creator/createdialog.cpp
#   documentsystem/creator/creatorcontroller.cpp
)
set( loadercontroller_SRCS
  documentsystem/loader/loaderactioncontroller.cpp
)
set( modifiedbarcontroller_SRCS
#   document/modified/modifiedbarcontroller.cpp
)
set( readonlycontroller_SRCS
#   document/readonly/readonlycontroller.cpp
)
set( versioncontroller_SRCS
#   view/version/versioncontroller.cpp
)
set( zoomcontroller_SRCS
#   view/zoom/zoomcontroller.cpp
)
set( selectcontroller_SRCS
#   view/select/selectcontroller.cpp
)
set( clipboardcontroller_SRCS
#   io/clipboard/clipboardcontroller.cpp
)
set( insertcontroller_SRCS
#   io/insert/insertdialog.cpp
#   io/insert/insertcontroller.cpp
)
set( copyascontroller_SRCS
#   io/copyas/copyasdialog.cpp
#   io/copyas/copyascontroller.cpp
)
set( exportcontroller_SRCS
#   io/export/exportdialog.cpp
#   io/export/exportcontroller.cpp
)
set( toollistmenucontroller_SRCS
  shellwindow/toollistmenu/toollistmenucontroller.cpp
)
set( fullscreencontroller_SRCS
#   shellwindow/fullscreen/fullscreencontroller.cpp
)

set( mobilekastencontroller_LIB_SRCS
  ${setremotecontroller_SRCS}
  ${versioncontroller_SRCS}
  ${modifiedbarcontroller_SRCS}
  ${readonlycontroller_SRCS}
  ${toollistmenucontroller_SRCS}
  ${fullscreencontroller_SRCS}
  ${zoomcontroller_SRCS}
  ${creatorcontroller_SRCS}
  ${loadercontroller_SRCS}
  ${synchronizecontroller_SRCS}
  ${selectcontroller_SRCS}
  ${clipboardcontroller_SRCS}
  ${insertcontroller_SRCS}
  ${copyascontroller_SRCS}
  ${exportcontroller_SRCS}
)

set( mobilekastencontrollers_LIB_HDRS
  mobilekastencontrollers_export.h
#   document/modified/modifiedbarcontroller.h
#   document/readonly/readonlycontroller.h
#   document/versionview/versionviewtool.h
#   documentsystem/close/closecontroller.h
#   documentsystem/creator/creatorcontroller.h
#   documentsystem/loader/loadercontroller.h
#   io/export/exportcontroller.h
#   io/insert/insertcontroller.h
#   io/setremote/setremotecontroller.h
#   io/copyas/copyascontroller.h
#   io/clipboard/clipboardcontroller.h
#   io/synchronize/synchronizecontroller.h
#   view/select/selectcontroller.h
#   view/zoom/zoomcontroller.h
#   view/zoom/zoombarcontroller.h
#   view/version/versioncontroller.h
)


kde4_add_library( kasten2mobilecontrollers SHARED ${mobilekastencontroller_LIB_SRCS} )
target_link_libraries( kasten2mobilecontrollers
  kasten2mobilegui
  kasten2controllers
  kasten2gui
  kasten2core
  ${KDE4_KIO_LIBS}
  ${KDE4_KPARTS_LIBS}
  ${KDE4_KFILE_LIBS}
)
set_target_properties( kasten2mobilecontrollers  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS kasten2mobilecontrollers  ${INSTALL_TARGETS_DEFAULT_ARGS} )

# install( FILES ${mobilekastencontrollers_LIB_HDRS}  DESTINATION  ${INCLUDE_INSTALL_DIR}/kasten  COMPONENT Devel )
