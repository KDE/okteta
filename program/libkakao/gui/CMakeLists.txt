project( kakaogui )

include_directories(
  ../core/document
  ../core/system
  view
  system
  controller
)

set( KAKAO_GUI_SRCS
  view/kabstractview.cpp
  view/abstracttoolview.cpp
  system/dummyview.cpp
  system/kviewmanager.cpp
  system/abstractgroupedviews.cpp
  system/tabbedviews.cpp
  system/shellwindow.cpp
  system/parallelviews/parallelviews.cpp
  system/parallelviews/parallelwidget.cpp
  system/parallelviews/titledwidget.cpp
  controller/abstractxmlguicontroller.cpp
)

kde4_add_library( kakaogui STATIC ${KAKAO_GUI_SRCS} )
