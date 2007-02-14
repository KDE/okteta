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
  kbytearraymodel_p.cpp
  kbytearraymodel.cpp
  kfixedsizebytearraymodel.cpp
  kfilebytearraymodel_p.cpp
  kfilebytearraymodel.cpp
  kwordbufferservice.cpp
  ksection.cpp
  ksectionlist.cpp)

kde4_automoc(${khexeditcore_LIB_SRCS})

kde4_add_library( khexeditcore SHARED ${khexeditcore_LIB_SRCS} )
target_link_libraries( khexeditcore  ${KDE4_KDECORE_LIBS} )
set_target_properties( khexeditcore PROPERTIES VERSION 1.0.0 SOVERSION 1 )


install(TARGETS khexeditcore  DESTINATION ${LIB_INSTALL_DIR})

if(KDE4_BUILD_TESTS)
   kde4_add_library( testkhexeditcore STATIC ${khexeditcore_LIB_SRCS} )
endif(KDE4_BUILD_TESTS)

