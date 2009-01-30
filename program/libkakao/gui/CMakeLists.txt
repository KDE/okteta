project( kakaogui )

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

set( KAKAO_GUI_PARALLELVIEWS_SRCS
  system/parallelviews/parallelviews.cpp
  system/parallelviews/parallelwidget.cpp
  system/parallelviews/titledwidget.cpp
)

set( KAKAO_GUI_IO_CONFIGEDITOR_SRCS
  io/filesystem/modelencoderfilesystemexporterconfigeditorfactory.cpp
  io/filesystem/modelencoderfilesystemexporterconfigeditor.cpp
  io/abstractmodelstreamencoderconfigeditor.cpp
  io/abstractmodelexporterconfigeditor.cpp
)

set( KAKAO_GUI_CONTROLLER_WIDGETS_SRCS
  controller/togglebutton_p.cpp
  controller/togglebutton.cpp
  controller/abstractlinepopup_p.cpp
  controller/abstractlinepopup.cpp
)

set( KAKAO_GUI_SRCS
  ${KAKAO_GUI_IO_CONFIGEDITOR_SRCS}
  view/kabstractview.cpp
  view/abstractselectionview.cpp
  view/abstracttoolview.cpp
  system/toolviewdockwidget.cpp
  system/dummyview.cpp
  system/modelcodecviewmanager.cpp
  system/kviewmanager.cpp
  system/abstractgroupedviews.cpp
  system/tabbedviews.cpp
  system/shellwindow.cpp
  ${KAKAO_GUI_CONTROLLER_WIDGETS_SRCS}
  controller/abstractxmlguicontroller.cpp
)

kde4_add_library( kakaogui STATIC ${KAKAO_GUI_SRCS} )
