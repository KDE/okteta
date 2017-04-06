ecm_setup_version( ${OKTETALIBS_LIB_VERSION}
    VARIABLE_PREFIX OKTETACORE
    SOVERSION ${OKTETALIBS_LIB_SOVERSION}
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/OktetaCoreConfigVersion.cmake"
)

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
    ArrayChangeMetricsList
    ByteArrayChange

    Bookmark
    BookmarksConstIteratorAdapter # TODO: make private or move adapter class to iterator
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


add_library( OktetaCore SHARED ${oktetacore_LIB_OBJS} )

generate_export_header( OktetaCore EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/okteta/oktetacore_export.h)

target_link_libraries( OktetaCore
PUBLIC
  Qt5::Core
PRIVATE
  KF5::I18n
  KF5::Codecs   #needed for codecs
)

target_include_directories(OktetaCore PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

set_target_properties( OktetaCore  PROPERTIES
  EXPORT_NAME "OktetaCore"
  OUTPUT_NAME okteta${OKTETALIBS_ABI_VERSION}core
  VERSION     ${OKTETACORE_VERSION}
  SOVERSION   ${OKTETACORE_SOVERSION}
)

install( TARGETS OktetaCore EXPORT OktetaCoreTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES
    ${oktetacore_LIB_HDRS}
    ${CMAKE_CURRENT_BINARY_DIR}/okteta/oktetacore_export.h
  DESTINATION ${OKTETALIBS_INCLUDE_INSTALL_DIR}
  COMPONENT Devel
)
install( FILES ${oktetacore_LIB_CCHDRS}
  DESTINATION ${OKTETALIBS_CCINCLUDE_INSTALL_DIR}
  COMPONENT Devel
)

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/OktetaCore")

ecm_configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/OktetaCoreConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/OktetaCoreConfig.cmake"
  INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install( FILES
    "${CMAKE_CURRENT_BINARY_DIR}/OktetaCoreConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/OktetaCoreConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT OktetaCoreTargets
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  FILE OktetaCoreTargets.cmake
)


if(BUILD_TESTING)
  add_library( testoktetacore STATIC ${oktetacore_LIB_OBJS} )
  set_target_properties( testoktetacore PROPERTIES COMPILE_FLAGS -DOKTETACORE_STATIC_DEFINE )
  target_include_directories( testoktetacore PUBLIC ${CMAKE_CURRENT_BINARY_DIR} )
  target_link_libraries( testoktetacore Qt5::Core KF5::I18n KF5::Codecs )
  add_subdirectory( piecetable/tests )
  add_subdirectory( tests )
endif()
