project( kastengui )

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=1804 )

include_directories(
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

set( KASTEN_GUI_SRCS
  ${KASTEN_GUI_IO_CONFIGEDITOR_SRCS}
  view/kabstractview.cpp
  view/abstractselectionview.cpp
  view/abstracttoolinlineview.cpp
  view/abstracttoolview.cpp
  system/viewbox.cpp
  system/toolinlineviewwidget.cpp
  system/toolviewdockwidget.cpp
  system/dummyview.cpp
  system/modelcodecviewmanager.cpp
  system/kviewmanager.cpp
  system/abstractgroupedviews.cpp
  system/tabbedviewsbox.cpp
  system/tabbedviews.cpp
  system/shellwindow.cpp
  system/statusbar.cpp
  system/statusbarlayout.cpp
  ${KASTEN_GUI_CONTROLLER_WIDGETS_SRCS}
  controller/abstractxmlguicontroller.cpp
)

kde4_add_library( kastengui STATIC ${KASTEN_GUI_SRCS} )
