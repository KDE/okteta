project( oktetacore )

add_definitions(-DTRANSLATION_DOMAIN=\"liboktetacore\")

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
  codecs/usasciicharcodec.cpp
  codecs/ebcdic1047charcodec.cpp
  codecs/jisx0201charcodec.cpp
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

ecm_generate_headers(oktetacore_LIB_CCHDRS
  HEADER_NAMES
    OktetaCore
    Range
    NumberRange

    Address
    AddressRange
    Size
    Byte
    AbstractByteArrayModel
    Versionable
    Bookmarkable
    ChangesDescribable
    ChangeHistory

    ByteArrayModel
    #FileByteArrayModel
    PieceTableByteArrayModel

    Character
    ArrayChangeMetrics
    ByteArrayChange

    Bookmark
    BookmarksConstIterator

    WordByteArrayService
    ValueCodec
    CharCodec
  PREFIX Okteta
  REQUIRED_HEADERS oktetacore_LIB_HDRS
)
#don't compile sources twice by using object libraries
#add_library( oktetacore_objs OBJECT ${oktetacore_LIB_SRCS} )
#target_include_directories( oktetacore_objs PUBLIC $<TARGET_PROPERTY:Qt5::Core,INTERFACE_INCLUDE_DIRECTORIES> )
#set( oktetacore_LIB_OBJS $<TARGET_OBJECTS:oktetacore_objs>)

# compile flags don't get set for object libraries, since cannot link to target
#TODO: find out how to properly use targets for object libraries
set( oktetacore_LIB_OBJS ${oktetacore_LIB_SRCS} )


add_library( ${oktetacore_LIB} SHARED ${oktetacore_LIB_OBJS} )

generate_export_header( ${oktetacore_LIB} )

target_link_libraries( ${oktetacore_LIB}
PUBLIC
  Qt5::Core
PRIVATE
  KF5::I18n
  KF5::Codecs   #needed for codecs
)

target_include_directories(${oktetacore_LIB} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/okteta>")

set_target_properties( ${oktetacore_LIB}  PROPERTIES
  OUTPUT_NAME ${oktetacore_LIB_NAME}
  VERSION     ${OKTETALIBS_LIB_VERSION}
  SOVERSION   ${OKTETALIBS_LIB_SOVERSION}
)

install( TARGETS ${oktetacore_LIB}  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES
    ${oktetacore_LIB_HDRS}
    bookmarksconstiteratoradapter.h # TODO: make private or move adapter class to iterator
    ${CMAKE_CURRENT_BINARY_DIR}/oktetacore_export.h
  DESTINATION ${OKTETALIBS_INCLUDE_INSTALL_DIR}
  COMPONENT Devel
)
install( FILES ${oktetacore_LIB_CCHDRS}
  DESTINATION ${OKTETALIBS_CCINCLUDE_INSTALL_DIR}
  COMPONENT Devel
)


if(BUILD_TESTING)
  add_library( testoktetacore STATIC ${oktetacore_LIB_OBJS} )
  set_target_properties( testoktetacore PROPERTIES COMPILE_FLAGS -DOKTETACORE_STATIC_DEFINE )
  target_include_directories( testoktetacore PUBLIC ${CMAKE_CURRENT_BINARY_DIR} )
  target_link_libraries( testoktetacore Qt5::Core KF5::I18n KF5::Codecs )
  add_subdirectory( piecetable/tests )
  add_subdirectory( tests )
endif()
