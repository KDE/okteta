set(OKTETAKASTENCONTROLLERS_VERSION ${OKTETAKASTEN_LIB_VERSION})
set(OKTETAKASTENCONTROLLERS_SO_VERSION ${OKTETAKASTEN_LIB_SOVERSION})

okteta_add_library(Controllers NAMESPACE Okteta Kasten
    PUBLIC
        OktetaKastenGui
        OktetaKastenCore
        KastenGui
        KastenCore
    PRIVATE
        framesprint
        KF5::NewStuffWidgets
        KF5::I18n
        KF5::ConfigWidgets
        KF5::ItemViews
        KF5::Completion
        KF5::KIOCore
        Qt5::PrintSupport
        Qt5::Script
        Qt5::Xml
#         Qt5::Test # QAbstractItemModelTester
    VERSION     ${OKTETAKASTENCONTROLLERS_VERSION}
    SOVERSION   ${OKTETAKASTENCONTROLLERS_SO_VERSION}
    ABIVERSION  ${OKTETAKASTEN_ABI_VERSION} ${KASTEN_ABI_VERSION}
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
    REVERSE_NAMESPACE_INCLUDEDIR
    REVERSE_NAMESPACE_LIB
    NO_TARGET_NAMESPACE
    NO_VERSIONED_EXPORTED_TARGET_NAME
    KI18N_TRANSLATION_DOMAIN "liboktetakasten"
)

ecm_qt_declare_logging_category(OktetaKastenControllers
    HEADER "checksumlogging.hpp"
    IDENTIFIER "LOG_OKTETA_KASTEN_CONTROLLER_CHECKSUM"
    CATEGORY_NAME "kasten.okteta.controllers.checksum"
    DESCRIPTION "Okteta Checksum tool"
    EXPORT OKTETA
)

macro(OKTETAKASTENCONTROLLERS_LIBRARY_SOURCES _subdir)
    okteta_library_sources(Controllers NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

macro(OKTETAKASTENCONTROLLERS_ADD_SUBLIBRARY _library_name)
    okteta_add_sublibrary(Controllers NAMESPACE Okteta Kasten
        SUBLIBRARY ${_library_name}
        ${ARGN}
    )
endmacro()

macro(OKTETAKASTENCONTROLLERS_SUBLIBRARY_SOURCES _subdir)
    okteta_sublibrary_sources(Controllers NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

oktetakastencontrollers_add_sublibrary(configentries
    NO_EXPORTS
    PUBLIC
        OktetaKastenGui
        KF5::ConfigCore
)

oktetakastencontrollers_add_sublibrary(datatypes
    NO_EXPORTS
    PUBLIC
        OktetaGui
    PRIVATE
        KF5::Completion
)

oktetakastencontrollers_add_sublibrary(datatypecodecs
    NO_EXPORTS
    PUBLIC
        datatypes
    PRIVATE
        KF5::I18n
    KI18N_TRANSLATION_DOMAIN "liboktetakasten"
)

oktetakastencontrollers_add_sublibrary(datatypedisplaystringifiers
    NO_EXPORTS
    PUBLIC
        datatypes
)

oktetakastencontrollers_add_sublibrary(datatypeeditors
    NO_EXPORTS
    PUBLIC
        datatypes
    PRIVATE
        KF5::Completion
)

oktetakastencontrollers_add_sublibrary(bytearraychecksum
    NO_EXPORTS
    PUBLIC
        OktetaCore
        Qt5::Widgets
    PRIVATE
        configentries
        KF5::Completion
        KF5::I18n
    KI18N_TRANSLATION_DOMAIN "liboktetakasten"
)

oktetakastencontrollers_add_sublibrary(bytearrayfilter
    NO_EXPORTS
    PUBLIC
        OktetaCore
        Qt5::Widgets
    PRIVATE
        configentries
        KF5::I18n
    KI18N_TRANSLATION_DOMAIN "liboktetakasten"
)

oktetakastencontrollers_add_sublibrary(finddialog
    NO_EXPORTS
    PUBLIC
        OktetaKastenGui
    PRIVATE
        KF5::I18n
    KI18N_TRANSLATION_DOMAIN "liboktetakasten"
)

oktetakastencontrollers_add_sublibrary(bytearraysearch
    NO_EXPORTS
    PUBLIC
        finddialog
)

oktetakastencontrollers_add_sublibrary(structuresdatatypes
    NO_EXPORTS
    PUBLIC
        OktetaCore
        KF5::ConfigGui
        Qt5::Script
    PRIVATE
        configentries
        datatypeeditors
        KF5::Completion
        KF5::I18n
    KI18N_TRANSLATION_DOMAIN "liboktetakasten"
)

# ecm_qt_declare_logging_category(structuresdatatypes
okteta_qt_declare_logging_category(Controllers NAMESPACE Okteta Kasten
    SUBLIBRARY structuresdatatypes
    HEADER "structureslogging.hpp"
    IDENTIFIER "LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES"
    CATEGORY_NAME "kasten.okteta.controllers.structures"
    DESCRIPTION "Okteta Structures tool"
    EXPORT OKTETA
)

oktetakastencontrollers_sublibrary_sources(libconfigentries
    SUBLIBRARY configentries
    BUILD_INCLUDEDIR
    PRIVATE
        AddressComboBoxCodingConfigEntry
        ByteArrayCodingConfigEntry
        ByteArrayComboBoxCodingConfigEntry
        CaseSensitivityConfigEntry
        DirectionConfigEntry
        EndiannessConfigEntry
        FindDirectionConfigEntry
)

oktetakastencontrollers_library_sources(document/info
    PUBLIC
        DocumentInfoToolFactory
        DocumentInfoToolViewFactory
    PRIVATE
        ByteArrayModelIoDevice
        DocumentInfoView
        DocumentInfoTool
        DocumentInfoToolView
)

# unused, TODO: remove?
if (FALSE)
oktetakastencontrollers_library_sources(document/overwriteonly
    PUBLIC
        OverwriteOnlyControllerFactory
    PRIVATE
        OverwriteOnlyController
)
endif()

oktetakastencontrollers_library_sources(view/overwritemode
    PUBLIC
        OverwriteModeControllerFactory
    PRIVATE
        OverwriteModeController
)
oktetakastencontrollers_library_sources(view/info
    PUBLIC
        InfoToolFactory
        InfoToolViewFactory
    PRIVATE
        CreateStatisticJob
        StatisticDisplayModel
        StatisticTableModel
        InfoView
        InfoTool
        InfoToolView
    KCFG
        infoviewsettings.kcfgc
)

oktetakastencontrollers_library_sources(view/stringsextract
    PUBLIC
        StringsExtractToolFactory
        StringsExtractToolViewFactory
    PRIVATE
        ExtractStringsJob
        ContainedStringTableModel
        StringsExtractView
        ContainedString
        StringsExtractTool
        StringsExtractToolView
)

oktetakastencontrollers_sublibrary_sources(view/libbytearraychecksum
    BUILD_INCLUDEDIR
    SUBLIBRARY bytearraychecksum
    PRIVATE
        AbstractByteArrayChecksumAlgorithm
        AbstractByteArrayChecksumParameterSet
        AbstractByteArrayChecksumParameterSetEdit
        ByteArrayChecksumAlgorithmFactory
        ByteArrayChecksumParameterSetEditFactory
)

oktetakastencontrollers_sublibrary_sources(view/libbytearraychecksum/algorithm
    SUBLIBRARY bytearraychecksum
    PRIVATE
        Crc32ByteArrayChecksumAlgorithm
        Crc64ByteArrayChecksumAlgorithm
        Adler32ByteArrayChecksumAlgorithm
        ModSum8ByteArrayChecksumAlgorithm
        ModSum16ByteArrayChecksumAlgorithm
        ModSum32ByteArrayChecksumAlgorithm
        ModSum64ByteArrayChecksumAlgorithm
        QCryptographicByteArrayChecksumAlgorithm
## NEWCHECKSUM(start)
## Here add the names of your classes of your checksum algorithm,
## e.g.
##         MyByteArrayChecksumAlgorithm
## NEWCHECKSUM(end)
        NoByteArrayChecksumParameterSet
        NoByteArrayChecksumParameterSetEdit
        ModSumByteArrayChecksumParameterSet
        ModSumByteArrayChecksumParameterSetEdit
        Crc64ByteArrayChecksumParameterSet
        Crc64ByteArrayChecksumParameterSetEdit
## NEWCHECKSUMPARAMETERSET(start)
## Here add the names of your classes of your checksum parameter set and
## of the widget to edit the checksum parameter set,
## e.g.
##         MyByteArrayChecksumParameterSet
##         MyByteArrayChecksumParameterSetEdit
## NEWCHECKSUMPARAMETERSET(end)
)

oktetakastencontrollers_library_sources(view/checksum
    PUBLIC
        ChecksumToolFactory
        ChecksumToolViewFactory
    PRIVATE
        ChecksumCalculateJob
        ChecksumView
        ChecksumTool
        ChecksumToolView
)

oktetakastencontrollers_sublibrary_sources(view/libfinddialog
    BUILD_INCLUDEDIR
    SUBLIBRARY finddialog
    PRIVATE
        AbstractFindDialog
        FindDirection
)

oktetakastencontrollers_sublibrary_sources(view/libbytearraysearch
    BUILD_INCLUDEDIR
    SUBLIBRARY bytearraysearch
    PRIVATE
        ByteArraySearchJob
        ByteArraySearchUtils
)

oktetakastencontrollers_sublibrary_sources(view/libdatatypes
    BUILD_INCLUDEDIR
    SUBLIBRARY datatypes
    PRIVATE
        Binary8
        Octal8
        Hexadecimal8
        UInt8
        UInt16
        UInt32
        UInt64
        SInt8
        SInt16
        SInt32
        SInt64
        Float32
        Float64
        Char8
        Utf8
        Utf16
)

oktetakastencontrollers_sublibrary_sources(view/libdatatypedisplaystringifiers
    BUILD_INCLUDEDIR
    SUBLIBRARY datatypedisplaystringifiers
    PRIVATE
        AbstractDataTypeDisplayStringifier
        DataTypeDisplayStringifierManager
)

oktetakastencontrollers_sublibrary_sources(view/libdatatypeeditors
    BUILD_INCLUDEDIR
    SUBLIBRARY datatypeeditors
    PRIVATE
        AbstractValue8Editor
        Binary8Editor
        Octal8Editor
        Hexadecimal8Editor
        UIntSpinBox
        SIntSpinBox
        Float32Editor
        Float64Editor
        Char8Editor
        Char8StringParser
        Char8StringValidator
        Utf8Editor
        Utf16Editor
)

oktetakastencontrollers_sublibrary_sources(view/libdatatypecodecs
    BUILD_INCLUDEDIR
    SUBLIBRARY datatypecodecs
    PRIVATE
        PODData
        AbstractTypeCodec
        Binary8Codec
        Octal8Codec
        Hexadecimal8Codec
        UInt8Codec
        UInt16Codec
        UInt32Codec
        UInt64Codec
        SInt8Codec
        SInt16Codec
        SInt32Codec
        SInt64Codec
        Float32Codec
        Float64Codec
        Char8Codec
        Utf8Codec
        Utf16Codec
)

oktetakastencontrollers_library_sources(view/poddecoder
    PUBLIC
        PODDecoderToolFactory
        PODDecoderToolViewFactory
    PRIVATE
        PODDisplayModel
        PODTableModel
        PODDelegate
        PODTableView
        PODDecoderTool
        PODDecoderToolView
)

oktetakastencontrollers_library_sources(view/bytetable
    PUBLIC
        ByteTableToolFactory
        ByteTableToolViewFactory
    PRIVATE
        ByteTableModel
        ByteTableView
        ByteTableTool
        ByteTableToolView
    KCFG
        bytetableviewsettings.kcfgc
)

oktetakastencontrollers_library_sources(view/crop
    PUBLIC
        CropControllerFactory
    PRIVATE
        CropController
)

oktetakastencontrollers_library_sources(view/search
    PUBLIC
        SearchControllerFactory
    PRIVATE
        SearchUserQueryable
        SearchDialog
        SearchTool
        SearchController
)

oktetakastencontrollers_library_sources(view/replace
    PUBLIC
        ReplaceControllerFactory
    PRIVATE
        ReplaceUserQueryable
        ReplaceController
        ReplaceDialog
        ReplacePrompt
        ReplaceJob
        ReplaceTool
)

oktetakastencontrollers_sublibrary_sources(view/libbytearrayfilter
    BUILD_INCLUDEDIR
    SUBLIBRARY bytearrayfilter
    PRIVATE
        AbstractByteArrayFilter
        AbstractByteArrayFilterParameterSet
        AbstractByteArrayFilterParameterSetEdit
        ByteArrayFilterFactory
        ByteArrayFilterParameterSetEditFactory
)

oktetakastencontrollers_sublibrary_sources(view/libbytearrayfilter/filter
    SUBLIBRARY bytearrayfilter
    PRIVATE
        NoByteArrayFilterParameterSet
        OperandByteArrayFilterParameterSet
        ReverseByteArrayFilterParameterSet
        RotateByteArrayFilterParameterSet
## NEWFILTERPARAMETERSET(start)
## Here add the names of your classes of your filter parameter set,
## e.g.
##         MyByteArrayFilterParameterSet
## NEWFILTERPARAMETERSET(end)
        AndByteArrayFilter
        OrByteArrayFilter
        XorByteArrayFilter
        InvertByteArrayFilter
        ReverseByteArrayFilter
        RotateByteArrayFilter
        ShiftByteArrayFilter
## NEWFILTER(start)
## Here add the names of your classes of your filter,
## e.g.
##         MyByteArrayFilter
## NEWFILTER(end)
        NoByteArrayFilterParameterSetedit
        OperandByteArrayFilterParameterSetedit
        ReverseByteArrayFilterParameterSetedit
        RotateByteArrayFilterParameterSetedit
## NEWFILTERPARAMETERSET(start)
## Here add the names of your classes of your widget to edit the filter parameter set,
## e.g.
##         MyByteArrayFilterParameterSetedit
## NEWFILTERPARAMETERSET(end)
)
oktetakastencontrollers_library_sources(view/filter
    PUBLIC
        FilterToolFactory
        FilterToolViewFactory
    PRIVATE
        FilterJob
        FilterView
        FilterTool
        FilterToolView
)

oktetakastencontrollers_library_sources(view/charsetconversion
    PUBLIC
        CharsetConversionToolFactory
        CharsetConversionToolViewFactory
    PRIVATE
        CharsetConversionJob
        CharsetConversionView
        CharsetConversionTool
        CharsetConversionToolView
)

oktetakastencontrollers_library_sources(view/gotooffset
    PUBLIC
        GotoOffsetControllerFactory
    PRIVATE
        GotoOffsetTool
        GotoOffsetView
        GotoOffsetToolView
        GotoOffsetController
)

oktetakastencontrollers_library_sources(view/selectrange
    PUBLIC
        SelectRangeControllerFactory
    PRIVATE
        SelectRangeTool
        SelectRangeView
        SelectRangeToolView
        SelectRangeController
)

oktetakastencontrollers_library_sources(view/bookmarks
    PUBLIC
        BookmarksControllerFactory
    PRIVATE
        BookmarkEditPopup
        BookmarksController
)

oktetakastencontrollers_library_sources(view/bookmarks
    PUBLIC
        BookmarksToolFactory
        BookmarksToolViewFactory
    PRIVATE
        BookmarkListModel
        BookmarksView
        BookmarksTool
        BookmarksToolView
)


oktetakastencontrollers_sublibrary_sources(view/structures/datatypes
    SUBLIBRARY structuresdatatypes
    PRIVATE
        TopLevelDataInformation
        DataInformation
        DataInformationbase
        DummyDataInformation
        DataInformationwithchildren
        StructureDataInformation
        TaggedUnionDataInformation
        UnionDataInformation
        PrimitiveFactory
        PrimitiveDataType
)

oktetakastencontrollers_sublibrary_sources(view/structures/datatypes/array
    SUBLIBRARY structuresdatatypes
    PRIVATE
        ArrayDataInformation
        AbstractArrayData
        ComplexArrayData
        PrimitiveArrayData
)

oktetakastencontrollers_sublibrary_sources(view/structures/datatypes/primitive
    SUBLIBRARY structuresdatatypes
    PRIVATE
        PointerDataInformation
        CharDataInformation
        DoubleDataInformation
        FloatDataInformation
        SIntDataInformation
        UIntDataInformation
        BoolDataInformation
        FlagDataInformation
        PrimitiveDataInformation
        BasicPrimitiveDataInformation
        EnumDataInformation
        EnumDefinition
)

oktetakastencontrollers_sublibrary_sources(view/structures/datatypes/primitive/bitfield
    SUBLIBRARY structuresdatatypes
    PRIVATE
        AbstractBitfieldDataInformation
        BoolBitfieldDataInformation
        SignedBitfieldDataInformation
        UnsignedBitfieldDataInformation
)

oktetakastencontrollers_sublibrary_sources(view/structures/datatypes/strings
    SUBLIBRARY structuresdatatypes
    PRIVATE
        StringDataInformation
        StringData
        AsciiStringData
        Latin1StringData
        Utf8StringData
        Utf16StringData
        Utf32StringData
        EbcdicStringData
)

oktetakastencontrollers_sublibrary_sources(../../core/charcodecs
    SUBLIBRARY structuresdatatypes
    PRIVATE
        Ebcdic1047CharCodec
)

oktetakastencontrollers_sublibrary_sources(view/structures/script
    SUBLIBRARY structuresdatatypes
    PRIVATE
        ScriptEngineInitializer
        ScriptHandler
        ScriptHandlerInfo
        ScriptLogger
        ScriptLoggerView
        SafeReference
)

oktetakastencontrollers_sublibrary_sources(view/structures/script/classes
    SUBLIBRARY structuresdatatypes
    PRIVATE
        DefaultScriptClass
        ArrayScriptClass
        PrimitiveScriptClass
        EnumScriptClass
        PointerScriptClass
        BitfieldScriptClass
        StringScriptClass
        StructUnionScriptClass
)

oktetakastencontrollers_sublibrary_sources(view/structures/parsers
    SUBLIBRARY structuresdatatypes
    PRIVATE
        AbstractStructureParser
        OsdParser
        ScriptFileParser
        ScriptValueConverter
        DataInformationFactory
        ParserUtils
)

oktetakastencontrollers_sublibrary_sources(view/structures/settings
    SUBLIBRARY structuresdatatypes
    KCFG
        structureviewpreferences.kcfgc
)

oktetakastencontrollers_sublibrary_sources(view/structures
    SUBLIBRARY structuresdatatypes
    PRIVATE
        AllPrimitiveTypes
)

#these source files are not needed by the static library for unit tests, therefore keep them separate
oktetakastencontrollers_library_sources(view/structures/settings
    PRIVATE
        StructureAddRemoveWidget
        StructureItemDelegate
        StructureEnabledList
        StructuresSelectionModel
        StructuresSelectionSortFilterProxyModel
        StructuresSelectionDialog
        StructureViewSettingsWidget
        StructuresManagerView
        StructuresSelector
        AboutStructureDialog
        LicenseDialog
    UI structureviewsettingswidget.ui
)
oktetakastencontrollers_library_sources(view/structures
    PUBLIC
        StructuresToolFactory
        StructuresToolViewFactory
    PRIVATE
        StructuresManager
        StructuresTool
        StructureTreeModel
        StructureMetaData
        StructureDefinitionFile
        StructureView
        StructuresToolView
        StructureViewItemDelegate
)

oktetakastencontrollers_library_sources(view/print
    PUBLIC
        PrintControllerFactory
    PRIVATE
        PrintTool
        PrintJob
        AbstractColumnFrameRenderer
        AbstractByteArrayFrameRenderer
        PrintColumnStylist
        ByteArrayColumnFrameRenderer
        ByteArrayRowFrameRenderer
        PrintDialog
        PrintPreviewDialog
        PrintController
)
configure_file(view/print/printtool-config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/printtool-config.hpp @ONLY)

oktetakastencontrollers_library_sources(view/viewconfig
    PUBLIC
        ViewConfigControllerFactory
    PRIVATE
        BytesPerLineDialog
        BytesPerGroupDialog
        ViewConfigController
)

oktetakastencontrollers_library_sources(view/viewstatus
    PUBLIC
        ViewStatusControllerFactory
    PRIVATE
        ViewStatusController
)

oktetakastencontrollers_library_sources(view/viewmode
    PUBLIC
        ViewModeControllerFactory
    PRIVATE
        ViewModeController
)

oktetakastencontrollers_library_sources(view/viewprofiles
    PUBLIC
        ViewProfilesManageControllerFactory
        # TODO: two controllers in one subdir
        ViewProfileControllerFactory
    PRIVATE
        ViewProfileEdit
        ViewProfileEditDialog
        ViewProfileTableModel

        ViewProfilesManageDialog

        ViewProfileController
        ViewProfilesManageController
)

oktetakastencontrollers_library_sources(view/viewcontextmenu
    PUBLIC
        ViewContextMenuControllerFactory
    PRIVATE
        ViewContextMenuController
)

oktetakastencontrollers_library_sources(utils
    BUILD_INCLUDEDIR
    PRIVATE
        LabelledToolBarWidget
        LabelledWidgetsToolBar
)

install(FILES view/structures/kns/okteta-structures.knsrc  DESTINATION ${KDE_INSTALL_KNSRCDIR})

set( elf_structure_FILES
    view/structures/examples/okteta/structures/elf/metadata.desktop
    view/structures/examples/okteta/structures/elf/main.osd
)
install( FILES ${elf_structure_FILES}  DESTINATION  ${KDE_INSTALL_DATADIR}/okteta/structures/elf )

set( png_structure_FILES
    view/structures/examples/okteta/structures/png/metadata.desktop
    view/structures/examples/okteta/structures/png/main.osd
)
install( FILES ${png_structure_FILES}  DESTINATION  ${KDE_INSTALL_DATADIR}/okteta/structures/png )
install( FILES view/structures/examples/okteta/structures/uuid.js  DESTINATION  ${KDE_INSTALL_DATADIR}/okteta/structures/ )
install( FILES view/structures/settings/structureviewpreferences.kcfg  DESTINATION  ${KDE_INSTALL_KCFGDIR} )

configure_file( view/structures/struct2osd.in ${CMAKE_CURRENT_BINARY_DIR}/struct2osd @ONLY )
install( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/struct2osd DESTINATION ${KDE_INSTALL_BINDIR} )
install( FILES view/structures/gccxml-to-osd.xsl DESTINATION ${KDE_INSTALL_DATADIR}/okteta/structures/ )

okteta_add_cmakeconfig(Controllers NAMESPACE Okteta Kasten
    DEPS
        OktetaKastenGui
)


if( KASTEN_BUILD_INTERNAL_TEMPLATES )

add_library(ByteArrayFilterTemplates STATIC)
okteta_library_sources(ByteArrayFilterTemplates
    SUBDIR view/libbytearrayfilter/filter/template
    PRIVATE
        Template_ByteArrayFilter
        Template_ByteArrayFilterParameterSet
        Template_ByteArrayFilterParameterSetEdit
)
target_compile_definitions(ByteArrayFilterTemplates
    PRIVATE
        TRANSLATION_DOMAIN=\"liboktetakasten\"
)
target_link_libraries(ByteArrayFilterTemplates
    PUBLIC
        bytearrayfilter
    PRIVATE
        KF5::I18n
)

add_library(ByteArrayCheckSumTemplates STATIC)
okteta_library_sources(ByteArrayCheckSumTemplates
    SUBDIR view/libbytearraychecksum/algorithm/template
    PRIVATE
        Template_ByteArrayCheckSumAlgorithm
        Template_ByteArrayChecksumParameterSet
        Template_ByteArrayChecksumParameterSetEdit
)
target_compile_definitions(ByteArrayCheckSumTemplates
    PRIVATE
        TRANSLATION_DOMAIN=\"liboktetakasten\"
)
target_link_libraries(ByteArrayCheckSumTemplates
    PUBLIC
        bytearraychecksum
    PRIVATE
        KF5::I18n
)

endif()

if(BUILD_TESTING)
    add_subdirectory(view/libdatatypeeditors/tests)
    add_subdirectory(view/replace/tests)
    add_subdirectory(view/search/tests)
    add_subdirectory(view/structures/tests)
endif()
