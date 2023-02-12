set(OKTETACORE_VERSION ${OKTETALIBS_LIB_VERSION})
set(OKTETACORE_SO_VERSION ${OKTETALIBS_LIB_SOVERSION})

okteta_add_library(Core NAMESPACE Okteta
    PUBLIC
        Qt5::Core
    PRIVATE
        KF5::I18n
    VERSION     ${OKTETACORE_VERSION}
    SOVERSION   ${OKTETACORE_SO_VERSION}
    ABIVERSION  ${OKTETALIBS_ABI_VERSION}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
)

target_compile_definitions(OktetaCore PRIVATE -DTRANSLATION_DOMAIN=\"liboktetacore\")

ecm_qt_declare_logging_category(OktetaCore
    HEADER "logging.hpp"
    IDENTIFIER "LOG_OKTETA_CORE"
    CATEGORY_NAME "okteta.core"
    DESCRIPTION "Okteta Core library"
    EXPORT OKTETA
)

macro(OKTETACORE_ADD_SUBLIBRARY _subdir)
    okteta_add_sublibrary(Core NAMESPACE Okteta
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

oktetacore_add_sublibrary(charcodecs
    PRIVATE
        CharCodec
        USASCIICharCodec
        EBCDIC1047CharCodec
        IBM874CharCodec
        JISX0201CharCodec
        TextCharCodec
)

oktetacore_add_sublibrary(valuecodecs
    PRIVATE
        BinaryByteCodec
        ValueCodec
        DecimalByteCodec
        HexadecimalByteCodec
        OctalByteCodec
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

okteta_add_cmakeconfig(Core NAMESPACE Okteta
    DEPS
        Qt5Core ${Qt5_VERSION}
)

okteta_add_qmakeconfig(Core NAMESPACE Okteta
    DEPS core
)

okteta_add_pkgconfig(Core NAMESPACE Okteta
    DEPS Qt5Core
    DESCRIPTION "Okteta Core"
)


if(BUILD_TESTING)
    #don't compile sources twice by using object libraries
    #add_library( oktetacore_objs OBJECT ${oktetacore_LIB_SRCS} )
    #target_include_directories( oktetacore_objs PUBLIC $<TARGET_PROPERTY:Qt5::Core,INTERFACE_INCLUDE_DIRECTORIES> )
    #set( oktetacore_LIB_OBJS $<TARGET_OBJECTS:oktetacore_objs>)

    # compile flags don't get set for object libraries, since cannot link to target
    #TODO: find out how to properly use targets for object libraries
    add_library( testoktetacore STATIC)
    get_property(oktetacore_LIB_OBJS TARGET OktetaCore PROPERTY SOURCES)
    target_sources(testoktetacore PRIVATE ${oktetacore_LIB_OBJS})
    set_target_properties(testoktetacore PROPERTIES COMPILE_FLAGS -DOKTETACORE_STATIC_DEFINE)
    target_include_directories(testoktetacore PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries(testoktetacore Qt5::Core KF5::I18n)

    add_subdirectory(piecetable/tests)
    add_subdirectory(tests)
endif()
