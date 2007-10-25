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
  system/dummyview.cpp
  system/kviewmanager.cpp
  system/abstractgroupedviews.cpp
  system/tabbedviews.cpp
  system/shellwindow.cpp
  controller/kcontroller.cpp
)

kde4_add_library( kakaogui STATIC ${KAKAO_GUI_SRCS} )
