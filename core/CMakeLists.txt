set(OKTETACORE_VERSION ${OKTETALIBS_LIB_VERSION})
set(OKTETACORE_SO_VERSION ${OKTETALIBS_LIB_SOVERSION})

add_definitions(-DTRANSLATION_DOMAIN=\"liboktetacore\")

macro(OKTETACORE_ADD_SUBLIBRARY _subdir)
    okteta_add_sublibrary(Core NAMESPACE Okteta
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

oktetacore_add_sublibrary(codecs
    PRIVATE
        BinaryByteCodec
        ValueCodec
        CharCodec
        DecimalByteCodec
        USASCIICharCodec
        EBCDIC1047CharCodec
        JISX0201CharCodec
        HexadecimalByteCodec
        OctalByteCodec
        TextCharCodec
)

oktetacore_add_sublibrary(piecetable
    PRIVATE
        PieceList
        PieceTable
        AbstractPieceTableChange
#           FillPieceTableChange
        InsertPieceTableChange
        RemovePieceTableChange
        ReplacePieceTableChange
        SwapRangesPieceTableChange
        GroupPieceTableChange
        PieceTableChangehistory
        RevertablePiecetable
)

okteta_add_sublibrary(Core  NAMESPACE Okteta
    PUBLIC
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

        TextByteArrayAnalyzer
        ValueCodec
        CharCodec

    PRIVATE
        FileByteArrayModel
        FixedSizeByteArrayModel

        BookmarkListConstIteratorAdapter
        AddressRangeList
        BookmarkList

)

#don't compile sources twice by using object libraries
#add_library( oktetacore_objs OBJECT ${oktetacore_LIB_SRCS} )
#target_include_directories( oktetacore_objs PUBLIC $<TARGET_PROPERTY:Qt5::Core,INTERFACE_INCLUDE_DIRECTORIES> )
#set( oktetacore_LIB_OBJS $<TARGET_OBJECTS:oktetacore_objs>)

# compile flags don't get set for object libraries, since cannot link to target
#TODO: find out how to properly use targets for object libraries
set( oktetacore_LIB_OBJS ${oktetacore_LIB_SRCS} )


okteta_add_library(Core NAMESPACE Okteta
    PUBLIC
        Qt5::Core
    PRIVATE
        KF5::I18n
        KF5::Codecs   #needed for codecs
    VERSION     ${OKTETACORE_VERSION}
    SOVERSION   ${OKTETACORE_SO_VERSION}
    ABIVERSION  ${OKTETALIBS_ABI_VERSION}
    SOURCES   ${oktetacore_LIB_SRCS}
    HEADERS   ${oktetacore_LIB_HDRS}
    CCHEADERS ${oktetacore_LIB_CCHDRS}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
)

okteta_add_cmakeconfig(Core NAMESPACE Okteta
)

okteta_add_qmakeconfig(Core NAMESPACE Okteta
    DEPS core
)

okteta_add_pkgconfig(Core NAMESPACE Okteta
    DEPS Qt5Core
    DESCRIPTION "Okteta Core"
)


if(BUILD_TESTING)
  add_library( testoktetacore STATIC ${oktetacore_LIB_OBJS} )
  set_target_properties( testoktetacore PROPERTIES COMPILE_FLAGS -DOKTETACORE_STATIC_DEFINE )
  target_include_directories( testoktetacore PUBLIC ${CMAKE_CURRENT_BINARY_DIR} )
  target_link_libraries( testoktetacore Qt5::Core KF5::I18n KF5::Codecs )
  add_subdirectory( piecetable/tests )
  add_subdirectory( tests )
endif()
