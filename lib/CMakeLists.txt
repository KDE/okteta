add_subdirectory( controller )
add_subdirectory( tests )


set( libcontroller_SRCS
  controller/kchareditor.cpp
  controller/kcontroller.cpp
  controller/keditor.cpp
  controller/knavigator.cpp
  controller/ktabcontroller.cpp
  controller/kvalueeditor.cpp
)

set(libcodecs_SRCS
  codecs/kbinarybytecodec.cpp
  codecs/kbytecodec.cpp
  codecs/kcharcodec.cpp
  codecs/kdecimalbytecodec.cpp
  codecs/kebcdic1047charcodec.cpp
  codecs/khexadecimalbytecodec.cpp
  codecs/koctalbytecodec.cpp
  codecs/ktextcharcodec.cpp
)

set( khexeditcore_LIB_SRCS
  ${libcodecs_SRCS}
  kabstractbytearraymodel.cpp
  kbytearraymodel.cpp
  kfixedsizebytearraymodel.cpp
  kfilebytearraymodel.cpp
  kwordbufferservice.cpp
  ksection.cpp
  ksectionlist.cpp)

set( khexeditgui_LIB_SRCS
  ${libcontroller_SRCS}
  kcolumn.cpp
  kbordercolumn.cpp
  koffsetcolumn.cpp
  kbuffercolumn.cpp
  kvaluecolumn.cpp
  kcharcolumn.cpp
  kcolumnsview.cpp
  khexedit.cpp
  kbytesedit.cpp
  koffsetformat.cpp
  kbuffercursor.cpp
  kbufferlayout.cpp
  kbufferranges.cpp
  kcursor.cpp
  kbufferdrag.cpp
  kcoordrangelist.cpp
  kbordercoltextexport.cpp
  koffsetcoltextexport.cpp
  kbuffercoltextexport.cpp
  kvaluecoltextexport.cpp
  kcharcoltextexport.cpp )

kde4_automoc(${khexeditcore_LIB_SRCS} ${khexeditgui_LIB_SRCS})

kde4_add_library( khexeditcore SHARED ${khexeditcore_LIB_SRCS} )
target_link_libraries( khexeditcore  ${KDE4_KDECORE_LIBS} )
set_target_properties( khexeditcore PROPERTIES VERSION 1.0.0 SOVERSION 1 )


kde4_add_library( khexeditgui SHARED ${khexeditgui_LIB_SRCS} )
target_link_libraries( khexeditgui khexeditcore ${KDE4_KDECORE_LIBS} )
set_target_properties( khexeditgui PROPERTIES VERSION 1.0.0 SOVERSION 1 )


install(TARGETS khexeditcore khexeditgui  DESTINATION ${LIB_INSTALL_DIR})

if(KDE4_BUILD_TESTS)
   kde4_add_library( testkhexeditcommon STATIC ${khexeditcore_LIB_SRCS} ${khexeditgui_LIB_SRCS} )
endif(KDE4_BUILD_TESTS)

