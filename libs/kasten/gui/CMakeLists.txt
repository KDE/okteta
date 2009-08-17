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
  shell
  controller
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
  system/dummyview.cpp
  system/modelcodecviewmanager.cpp
  system/viewmanager.cpp
  shell/viewbox.cpp
  shell/toolinlineviewwidget.cpp
  shell/toolviewdockwidget.cpp
  shell/abstractviewarea.cpp
  shell/abstractgroupedviews.cpp
  shell/tabbedviewsbox.cpp
  shell/tabbedviews.cpp
  shell/multiviewareas.cpp
  shell/shellwindow.cpp
  shell/statusbar.cpp
  shell/statusbarlayout.cpp
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
  system/dummyview.h
  system/modelcodecviewmanager.h
  system/viewmanager.h
  shell/shellwindow.h
  shell/abstractviewarea.h
  shell/abstractgroupedviews.h
  shell/statusbar.h
  shell/tabbedviewsbox.h
  shell/viewbox.h
  shell/tabbedviews.h
  shell/multiviewareas.h
  shell/toolinlineviewwidget.h
  shell/toolviewdockwidget.h
  shell/statusbarlayout.h
  shell/viewfocusable.h
  shell/toolinlineviewable.h
  shell/widgetsdockable.h
  shell/viewareasplitable.h
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
