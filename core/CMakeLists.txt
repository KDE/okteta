project( oktetacore )

add_definitions( -DKDE_DEFAULT_DEBUG_AREA=1801 )

set( libcontroller_SRCS
  controller/kchareditor.cpp
  controller/kcontroller.cpp
  controller/keditor.cpp
  controller/knavigator.cpp
  controller/ktabcontroller.cpp
  controller/kvalueeditor.cpp
)

set(libcodecs_SRCS
  codecs/binarybytecodec.cpp
  codecs/valuecodec.cpp
  codecs/charcodec.cpp
  codecs/decimalbytecodec.cpp
  codecs/ebcdic1047charcodec.cpp
  codecs/hexadecimalbytecodec.cpp
  codecs/octalbytecodec.cpp
  codecs/textcharcodec.cpp
)

set( libpiecetable_SRCS
  piecetable/piecelist.cpp
  piecetable/piecetable.cpp
  piecetable/abstractpiecetablechange.cpp
#   piecetable/fillpiecetablechange.cpp
  piecetable/insertpiecetablechange.cpp
  piecetable/removepiecetablechange.cpp
  piecetable/replacepiecetablechange.cpp
  piecetable/swaprangespiecetablechange.cpp
  piecetable/grouppiecetablechange.cpp
  piecetable/piecetablechangehistory.cpp
  piecetable/revertablepiecetable.cpp
)

set( oktetacore_LIB_SRCS
  ${libcodecs_SRCS}
  ${libpiecetable_SRCS}
  oktetacore.cpp
  abstractbytearraymodel.cpp
  bytearraymodel_p.cpp
  bytearraymodel.cpp
  fixedsizebytearraymodel.cpp
  filebytearraymodel_p.cpp
  filebytearraymodel.cpp
  piecetablebytearraymodel_p.cpp
  piecetablebytearraymodel.cpp
  wordbytearrayservice.cpp
  bookmarklistconstiteratoradapter.cpp
  bookmarklist.cpp
  addressrangelist.cpp
)

set( oktetacore_LIB_HDRS
  oktetacore_export.h
  oktetacore.h
  address.h
  size.h
  byte.h
  abstractbytearraymodel.h
  piecetablebytearraymodel.h
#   filebytearraymodel.h
  bytearraymodel.h
  bookmarkable.h
  changesdescribable.h
  changehistory.h
  versionable.h
  arraychangemetrics.h
  bookmark.h
  bookmarksconstiterator.h
  bookmarksconstiteratoradapter.h
  bytearraychange.h
  charcodec.h
  character.h
  range.h
  numberrange.h
  addressrange.h
  valuecodec.h
  wordbytearrayservice.h
)

kde4_add_library( ${oktetacore_LIB} SHARED ${oktetacore_LIB_SRCS} )
target_link_libraries( ${oktetacore_LIB}
  ${KDE4_KDECORE_LIBS}
)
set_target_properties( ${oktetacore_LIB}  PROPERTIES
  VERSION ${OKTETALIBS_LIB_VERSION}
  SOVERSION ${OKTETALIBS_LIB_SOVERSION}
)

install( TARGETS ${oktetacore_LIB}  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${oktetacore_LIB_HDRS}
  DESTINATION ${OKTETALIBS_INCLUDE_INSTALL_DIR}
  COMPONENT Devel
)


add_subdirectory( piecetable/tests )
add_subdirectory( tests )
if(KDE4_BUILD_TESTS)
  kde4_add_library( testoktetacore STATIC ${oktetacore_LIB_SRCS} )
  set_target_properties( testoktetacore PROPERTIES COMPILE_FLAGS "-DOKTETACORE_EXPORT=" )
endif(KDE4_BUILD_TESTS)
