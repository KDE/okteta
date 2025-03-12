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
    NO_VERSIONED_EXPORTED_TARGET_NAME
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
    KI18N_TRANSLATION_DOMAIN "liboktetacore"
)

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

macro(OKTETACORE_SUBLIBRARY_SOURCES _subdir)
    okteta_sublibrary_sources(Core NAMESPACE Okteta
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

oktetacore_add_sublibrary(coretypes
    PUBLIC
        Qt5::Core
    PRIVATE
        KF5::I18n
    KI18N_TRANSLATION_DOMAIN "liboktetacore"
)

oktetacore_add_sublibrary(bookmark
    PUBLIC
        coretypes
)

oktetacore_add_sublibrary(charcodecs
    PUBLIC
        coretypes
)

oktetacore_add_sublibrary(valuecodecs
    PUBLIC
        coretypes
)

oktetacore_add_sublibrary(piecetable
    PUBLIC
        coretypes
    PRIVATE
        KF5::I18n
    KI18N_TRANSLATION_DOMAIN "liboktetacore"
)

oktetacore_add_sublibrary(bytearraymodels
    PUBLIC
        bookmark
        piecetable
)

okteta_sublibrary_sources(Core NAMESPACE Okteta
    SUBLIBRARY coretypes
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

okteta_sublibrary_sources(Core NAMESPACE Okteta
    SUBLIBRARY bookmark
    PUBLIC
        Bookmark
        BookmarksConstIteratorAdapter # TODO: make private or move adapter class to iterator
        BookmarksConstIterator

    PRIVATE
        BookmarkListConstIteratorAdapter
        BookmarkList
)

oktetacore_sublibrary_sources(charcodecs
    SUBLIBRARY charcodecs
    PRIVATE
        CharCodec
        USASCIICharCodec
        EBCDIC1047CharCodec
        IBM874CharCodec
        JISX0201CharCodec
        TextCharCodec
)

oktetacore_sublibrary_sources(valuecodecs
    SUBLIBRARY valuecodecs
    PRIVATE
        BinaryByteCodec
        ValueCodec
        DecimalByteCodec
        HexadecimalByteCodec
        OctalByteCodec
)

oktetacore_sublibrary_sources(piecetable
    SUBLIBRARY piecetable
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

okteta_sublibrary_sources(Core NAMESPACE Okteta
    SUBLIBRARY bytearraymodels
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

okteta_library_sources(Core NAMESPACE Okteta
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
    add_subdirectory(piecetable/tests)
    add_subdirectory(tests)
endif()
