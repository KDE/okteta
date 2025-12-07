set(OKTETACORE_VERSION ${OKTETALIBS_LIB_VERSION})
set(OKTETACORE_SO_VERSION ${OKTETALIBS_LIB_SOVERSION})

if (BUILD_TESTING)
    ecm_test_set_dir_properties(PREFIX_NAME "libokteta-core")
endif()

okteta_add_library(Core NAMESPACE Okteta
    PUBLIC
        Qt6::Core
        Qt6::Core5Compat # QLinkedList
    PRIVATE
        KF6::I18n
    VERSION     ${OKTETACORE_VERSION}
    SOVERSION   ${OKTETACORE_SO_VERSION}
    ABIVERSION  ${OKTETALIBS_ABI_VERSION}
    NO_TARGET_NAMESPACE
    NO_VERSIONED_INCLUDEDIR
    KI18N_TRANSLATION_DOMAIN "liboktetacore"
)

macro(OKTETACORE_SOURCES)
    okteta_library_sources(Core NAMESPACE Okteta
        ${ARGN}
    )
endmacro()

ecm_qt_declare_logging_category(OktetaCore
    HEADER "logging.hpp"
    IDENTIFIER "LOG_OKTETA_CORE"
    CATEGORY_NAME "okteta.core"
    DESCRIPTION "Okteta Core library"
    EXPORT OKTETA
)

macro(OKTETACORE_ADD_SUBLIBRARY _library_name)
    okteta_add_sublibrary(Core NAMESPACE Okteta
        SUBLIBRARY ${_library_name}
        ${ARGN}
    )
endmacro()

macro(OKTETACORE_SUBLIBRARY_SOURCES _library_name)
    okteta_sublibrary_sources(Core NAMESPACE Okteta
        SUBLIBRARY ${_library_name}
        ${ARGN}
    )
endmacro()

oktetacore_add_sublibrary(OktetaCoreTypes
    PUBLIC
        Qt6::Core
    PRIVATE
        KF6::I18n
        Qt6::Core5Compat # QLinkedList
    KI18N_TRANSLATION_DOMAIN "liboktetacore"
)

oktetacore_add_sublibrary(OktetaCoreBookmark
    PUBLIC
        OktetaCoreTypes
        Qt6::Core5Compat # QLinkedList
)

oktetacore_add_sublibrary(OktetaCoreCharCodecs
    PUBLIC
        OktetaCoreTypes
)

oktetacore_add_sublibrary(OktetaCoreValueCodecs
    PUBLIC
        OktetaCoreTypes
)

oktetacore_add_sublibrary(OktetaCorePieceTable
    PUBLIC
        OktetaCoreTypes
        Qt6::Core5Compat # QLinkedList
    PRIVATE
        KF6::I18n
    KI18N_TRANSLATION_DOMAIN "liboktetacore"
)

oktetacore_add_sublibrary(OktetaCoreByteArrayModels
    PUBLIC
        OktetaCoreBookmark
        OktetaCorePieceTable
)

oktetacore_sublibrary_sources(OktetaCoreTypes
    PUBLIC
        OktetaCore
        Range
        NumberRange

        Address
        AddressRange
        Size
        Byte

        ArrayChangeMetrics
        ArrayChangeMetricsList

        Character
        ByteArrayChange

    PRIVATE
        AddressRangeList
)

oktetacore_sublibrary_sources(OktetaCoreBookmark
    PUBLIC
        Bookmark
        BookmarksConstIteratorAdapter # TODO: make private or move adapter class to iterator
        BookmarksConstIterator

    PRIVATE
        BookmarkListConstIteratorAdapter
        BookmarkList
)

oktetacore_sublibrary_sources(OktetaCoreCharCodecs
    SUBDIR charcodecs
    PRIVATE
        CharCodec
        USASCIICharCodec
        EBCDIC1047CharCodec
        IBM874CharCodec
        JISX0201CharCodec
        TextCharCodec
)

oktetacore_sublibrary_sources(OktetaCoreValueCodecs
    SUBDIR valuecodecs
    PRIVATE
        BinaryByteCodec
        ValueCodec
        DecimalByteCodec
        HexadecimalByteCodec
        OctalByteCodec
)

oktetacore_sublibrary_sources(OktetaCorePieceTable
    SUBDIR piecetable
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

oktetacore_sublibrary_sources(OktetaCoreByteArrayModels
    PUBLIC
        AbstractByteArrayModel
        Versionable
        Bookmarkable
        ChangesDescribable
        ChangeHistory

        ByteArrayModel
        #FileByteArrayModel
        PieceTableByteArrayModel

    PRIVATE
        FileByteArrayModel
        FixedSizeByteArrayModel
)

oktetacore_sources(
    PUBLIC
        TextByteArrayAnalyzer
        # TODO: both repeated here as the headers are in the toplevel-dir
        # to hide the headers of the concrete implementations from include dirs
        # Should get a better approach.
        ValueCodec
        CharCodec
)

okteta_add_cmakeconfig(Core NAMESPACE Okteta
    DEPS
        Qt6Core ${Qt6_VERSION}
        Qt6Core5Compat ${Qt6_VERSION}
)

okteta_add_qmakeconfig(Core NAMESPACE Okteta
    DEPS core core5compat
)

okteta_add_pkgconfig(Core NAMESPACE Okteta
    DEPS Qt6Core Qt6Core5Compat
    DESCRIPTION "Okteta Core"
)


if(BUILD_TESTING)
    add_subdirectory(piecetable/tests)
    add_subdirectory(tests)
endif()
