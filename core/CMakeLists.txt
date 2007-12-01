project( oktetacore )

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

set( oktetacore_LIB_SRCS
  ${libcodecs_SRCS}
  kabstractbytearraymodel.cpp
  kbytearraymodel_p.cpp
  kbytearraymodel.cpp
  kfixedsizebytearraymodel.cpp
  kfilebytearraymodel_p.cpp
  kfilebytearraymodel.cpp
  kwordbufferservice.cpp
  kbookmarklist.cpp
  ksection.cpp
  ksectionlist.cpp
)

kde4_add_library( oktetacore SHARED ${oktetacore_LIB_SRCS} )
target_link_libraries( oktetacore  ${KDE4_KDECORE_LIBS} )
set_target_properties( oktetacore PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )


install( TARGETS oktetacore  DESTINATION ${LIB_INSTALL_DIR})

add_subdirectory( tests )
if(KDE4_BUILD_TESTS)
  kde4_add_library( testoktetacore STATIC ${oktetacore_LIB_SRCS} )
endif(KDE4_BUILD_TESTS)

