add_subdirectory( test )

include_directories(
  ${CMAKE_SOURCE_DIR}/khexedit/core
  ${CMAKE_SOURCE_DIR}/khexedit/gui/libkcolumnsview
)

set( libcontroller_SRCS
  controller/kchareditor.cpp
  controller/kcontroller.cpp
  controller/keditor.cpp
  controller/knavigator.cpp
  controller/ktabcontroller.cpp
  controller/kvalueeditor.cpp
)

set( libkcolumnsview_SRCS
  libkcolumnsview/kcolumn.cpp
  libkcolumnsview/kcolumnsview.cpp
)

set( khexeditgui_LIB_SRCS
  ${libkcolumnsview_SRCS}
  ${libcontroller_SRCS}
  kbordercolumn.cpp
  koffsetcolumn.cpp
  kdatacolumn.cpp
  kvaluecolumn.cpp
  kcharcolumn.cpp
  kbytearrayview.cpp
  koffsetformat.cpp
  kdatacursor.cpp
  kdatalayout.cpp
  kdataranges.cpp
  kcursor.cpp
  kbytearraydrag.cpp
  kcoordrangelist.cpp
  kbordercoltextexport.cpp
  koffsetcoltextexport.cpp
  kdatacoltextexport.cpp
  kvaluecoltextexport.cpp
  kcharcoltextexport.cpp )

kde4_automoc(${khexeditgui_LIB_SRCS})

kde4_add_library( khexeditgui SHARED ${khexeditgui_LIB_SRCS} )
target_link_libraries( khexeditgui khexeditcore ${KDE4_KDEUI_LIBS} )
set_target_properties( khexeditgui PROPERTIES VERSION 1.0.0 SOVERSION 1 )


install(TARGETS khexeditgui  DESTINATION ${LIB_INSTALL_DIR})

#if(KDE4_BUILD_TESTS)
#   kde4_add_library( testkhexeditcommon STATIC ${khexeditgui_LIB_SRCS} )
#endif(KDE4_BUILD_TESTS)

