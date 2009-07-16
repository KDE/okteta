project( kastengui )

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=1804 )

include_directories(
  ../core
  ../core/document
  ../core/io
  ../core/system
  view
  io
  system
  controller
)

set( KASTEN_GUI_PARALLELVIEWS_SRCS
  system/parallelviews/parallelviews.cpp
  system/parallelviews/parallelwidget.cpp
  system/parallelviews/titledwidget.cpp
)

set( KASTEN_GUI_IO_CONFIGEDITOR_SRCS
  io/filesystem/modelencoderfilesystemexporterconfigeditorfactory.cpp
  io/filesystem/modelencoderfilesystemexporterconfigeditor.cpp
  io/abstractmodelstreamencoderconfigeditor.cpp
  io/abstractmodelexporterconfigeditor.cpp
)

set( KASTEN_GUI_CONTROLLER_WIDGETS_SRCS
  controller/togglebutton_p.cpp
  controller/togglebutton.cpp
  controller/abstractlinepopup_p.cpp
  controller/abstractlinepopup.cpp
)

set( kastengui_LIB_SRCS
  ${KASTEN_GUI_IO_CONFIGEDITOR_SRCS}
  view/abstractview.cpp
  view/abstractselectionview.cpp
  view/abstracttoolinlineview.cpp
  view/abstracttoolview.cpp
  system/viewbox.cpp
  system/toolinlineviewwidget.cpp
  system/toolviewdockwidget.cpp
  system/dummyview.cpp
  system/modelcodecviewmanager.cpp
  system/viewmanager.cpp
  system/abstractgroupedviews.cpp
  system/tabbedviewsbox.cpp
  system/tabbedviews.cpp
  system/shellwindow.cpp
  system/statusbar.cpp
  system/statusbarlayout.cpp
  ${KASTEN_GUI_CONTROLLER_WIDGETS_SRCS}
  controller/abstractxmlguicontroller.cpp
)

set( kastengui_LIB_HDRS
  kastengui_export.h
  view/abstractviewfactory.h
  view/selecteddatawriteable.h
  view/abstracttoolinlineview.h
  view/abstractview.h
  view/abstractselectionview.h
  view/abstracttoolview.h
  view/dataselectable.h
  view/zoomable.h
  io/filesystem/modelencoderfilesystemexporterconfigeditor.h
  io/filesystem/modelencoderfilesystemexporterconfigeditorfactory.h
  io/abstractmodelstreamencoderconfigeditor.h
  io/abstractmodelexporterconfigeditor.h
  io/abstractmodelexporterconfigeditorfactory.h
  io/abstractmodelstreamencoderconfigeditorfactory.h
  system/viewmanager.h
  system/shellwindow.h
  system/abstractgroupedviews.h
  system/statusbar.h
  system/tabbedviewsbox.h
  system/viewbox.h
  system/tabbedviews.h
  system/toolinlineviewwidget.h
  system/toolviewdockwidget.h
  system/statusbarlayout.h
  system/dummyview.h
  system/modelcodecviewmanager.h
  system/viewfocusable.h
  system/toolinlineviewable.h
  system/widgetsdockable.h
  controller/abstractxmlguicontroller.h
  controller/abstractlinepopup.h
  controller/togglebutton.h
)

kde4_add_library( kastengui SHARED ${kastengui_LIB_SRCS} )
target_link_libraries( kastengui
  kastencore
  ${KDE4_KDEUI_LIBS}
)
set_target_properties( kastengui  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS kastengui  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${kastengui_LIB_HDRS}  DESTINATION  ${INCLUDE_INSTALL_DIR}/kasten  COMPONENT Devel )
