set(OKTETAKASTENCONTROLLERS_VERSION ${OKTETAKASTEN_LIB_VERSION})
set(OKTETAKASTENCONTROLLERS_SO_VERSION ${OKTETAKASTEN_LIB_SOVERSION})

set( LIBFRAMESPRINT_REL_DIR ../../libs/framesprint )

add_definitions(-DTRANSLATION_DOMAIN=\"liboktetakasten\")

set(HAVE_QCA2 ${Qca-qt5_FOUND})
configure_file(config-qca2.hpp.in  ${CMAKE_CURRENT_BINARY_DIR}/config-qca2.hpp)

macro(OKTETAKASTENCONTROLLERS_ADD_SUBLIBRARY _subdir)
    okteta_add_sublibrary(Controllers NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

oktetakastencontrollers_add_sublibrary(libconfigentries
    PRIVATE
        AddressComboBoxCodingConfigEntry
        ByteArrayCodingConfigEntry
        ByteArrayComboBoxCodingConfigEntry
        CaseSensitivityConfigEntry
        DirectionConfigEntry
        EndiannessConfigEntry
        FindDirectionConfigEntry
)

oktetakastencontrollers_add_sublibrary(document/info
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
oktetakastencontrollers_add_sublibrary(document/overwriteonly
    PUBLIC
        OverwriteOnlyControllerFactory
    PRIVATE
        OverwriteOnlyController
)
endif()

oktetakastencontrollers_add_sublibrary(view/overwritemode
    PUBLIC
        OverwriteModeControllerFactory
    PRIVATE
        OverwriteModeController
)
oktetakastencontrollers_add_sublibrary(view/info
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
configure_file(view/info/statisticdisplaymodel-config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/statisticdisplaymodel-config.hpp @ONLY)

oktetakastencontrollers_add_sublibrary(view/stringsextract
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

oktetakastencontrollers_add_sublibrary(view/libbytearraychecksum
    PRIVATE
        AbstractByteArrayChecksumAlgorithm
        AbstractByteArrayChecksumParameterSet
        AbstractByteArrayChecksumParameterSetEdit
        ByteArrayChecksumAlgorithmFactory
        ByteArrayChecksumParameterSetEditFactory
)
if( Qca-qt5_FOUND )
    set( QCA2CHECKSUM_CLASSES
        Qca2ByteArrayChecksumAlgorithm
    )
endif()
oktetakastencontrollers_add_sublibrary(view/libbytearraychecksum/algorithm
    PRIVATE
        Crc32ByteArrayChecksumAlgorithm
        Crc64ByteArrayChecksumAlgorithm
        Adler32ByteArrayChecksumAlgorithm
        ModSum8ByteArrayChecksumAlgorithm
        ModSum16ByteArrayChecksumAlgorithm
        ModSum32ByteArrayChecksumAlgorithm
        ModSum64ByteArrayChecksumAlgorithm
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
        ${QCA2CHECKSUM_CLASSES}
)

oktetakastencontrollers_add_sublibrary(view/checksum
    PUBLIC
        ChecksumToolFactory
        ChecksumToolViewFactory
    PRIVATE
        ChecksumCalculateJob
        ChecksumLogging
        ChecksumView
        ChecksumTool
        ChecksumToolView
)

oktetakastencontrollers_add_sublibrary(view/libfinddialog
    PUBLIC
        FindDirection
    PRIVATE
        AbstractFindDialog
)

oktetakastencontrollers_add_sublibrary(view/libbytearraysearch
    PRIVATE
        ByteArraySearchJob
        ByteArraySearchUtils
)

oktetakastencontrollers_add_sublibrary(view/poddecoder/typecodecs
    PRIVATE
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
)

oktetakastencontrollers_add_sublibrary(view/poddecoder
    PUBLIC
        PODDecoderToolFactory
        PODDecoderToolViewFactory
    PRIVATE
        PODData
        PODDisplayModel
        PODTableModel
        PODDelegate
        PODTableView
        PODDecoderTool
        PODDecoderToolView
)

oktetakastencontrollers_add_sublibrary(view/poddecoder/types
    SOURCE_TAG typeeditor
    PRIVATE
        Char8
)

oktetakastencontrollers_add_sublibrary(view/poddecoder/typeeditors
    SOURCE_TAG typeeditor
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
)

oktetakastencontrollers_add_sublibrary(view/bytetable
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

oktetakastencontrollers_add_sublibrary(view/search
    PUBLIC
        SearchControllerFactory
    PRIVATE
        SearchUserQueryable
        SearchDialog
        SearchTool
        SearchController
)

oktetakastencontrollers_add_sublibrary(view/replace
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

oktetakastencontrollers_add_sublibrary(view/libbytearrayfilter
    PRIVATE
        AbstractByteArrayFilter
        AbstractByteArrayFilterParameterSet
        AbstractByteArrayFilterParameterSetEdit
        ByteArrayFilterFactory
        ByteArrayFilterParameterSetEditFactory
)

oktetakastencontrollers_add_sublibrary(view/libbytearrayfilter/filter
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
oktetakastencontrollers_add_sublibrary(view/filter
    PUBLIC
        FilterToolFactory
        FilterToolViewFactory
    PRIVATE
        FilterJob
        FilterView
        FilterTool
        FilterToolView
)

oktetakastencontrollers_add_sublibrary(view/charsetconversion
    PUBLIC
        CharsetConversionToolFactory
        CharsetConversionToolViewFactory
    PRIVATE
        CharsetConversionJob
        CharsetConversionView
        CharsetConversionTool
        CharsetConversionToolView
)

oktetakastencontrollers_add_sublibrary(view/gotooffset
    PUBLIC
        GotoOffsetControllerFactory
    PRIVATE
        GotoOffsetTool
        GotoOffsetView
        GotoOffsetToolView
        GotoOffsetController
)

oktetakastencontrollers_add_sublibrary(view/selectrange
    PUBLIC
        SelectRangeControllerFactory
    PRIVATE
        SelectRangeTool
        SelectRangeView
        SelectRangeToolView
        SelectRangeController
)

oktetakastencontrollers_add_sublibrary(view/bookmarks
    PUBLIC
        BookmarksControllerFactory
    PRIVATE
        BookmarkEditPopup
        BookmarksController
)

oktetakastencontrollers_add_sublibrary(view/bookmarks
    PUBLIC
        BookmarksToolFactory
        BookmarksToolViewFactory
    PRIVATE
        BookmarkListModel
        BookmarksView
        BookmarksTool
        BookmarksToolView
)


oktetakastencontrollers_add_sublibrary(view/structures/datatypes
    SOURCE_TAG structuresdatatypes
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

oktetakastencontrollers_add_sublibrary(view/structures/datatypes/array
    SOURCE_TAG structuresdatatypes
    PRIVATE
        ArrayDataInformation
        AbstractArrayData
        ComplexArrayData
        PrimitiveArrayData
)

oktetakastencontrollers_add_sublibrary(view/structures/datatypes/primitive
    SOURCE_TAG structuresdatatypes
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

oktetakastencontrollers_add_sublibrary(view/structures/datatypes/primitive/bitfield
    SOURCE_TAG structuresdatatypes
    PRIVATE
        AbstractBitfieldDataInformation
        BoolBitfieldDataInformation
        SignedBitfieldDataInformation
        UnsignedBitfieldDataInformation
)

oktetakastencontrollers_add_sublibrary(view/structures/datatypes/strings
    SOURCE_TAG structuresdatatypes
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
oktetakastencontrollers_add_sublibrary(../../core/codecs
    SOURCE_TAG structuresdatatypes
    PRIVATE
        Ebcdic1047CharCodec
)

oktetakastencontrollers_add_sublibrary(view/structures/script
    SOURCE_TAG structuresdatatypes
    PRIVATE
        ScriptEngineInitializer
        ScriptHandler
        ScriptHandlerInfo
        ScriptLogger
        ScriptLoggerView
        SafeReference
)

oktetakastencontrollers_add_sublibrary(view/structures/script/classes
    SOURCE_TAG structuresdatatypes
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

oktetakastencontrollers_add_sublibrary(view/structures/parsers
    SOURCE_TAG structuresdatatypes
    PRIVATE
        AbstractStructureParser
        OsdParser
        ScriptFileParser
        ScriptValueConverter
        DataInformationFactory
        ParserUtils
)

oktetakastencontrollers_add_sublibrary(view/structures/settings
    SOURCE_TAG structuresdatatypes
    KCFG
        structureviewpreferences.kcfgc
)

oktetakastencontrollers_add_sublibrary(view/structures
    SOURCE_TAG structuresdatatypes
    PRIVATE
        AllPrimitiveTypes
        StructLogging
)

#these source files are not needed by the static library for unit tests, therefore keep them separate
oktetakastencontrollers_add_sublibrary(view/structures/settings
    PRIVATE
        StructureAddRemoveWidget
        StructuresSelectionDialog
        StructureViewSettingsWidget
        StructuresManagerView
    UI structureviewsettingswidget.ui
)
oktetakastencontrollers_add_sublibrary(view/structures
    PUBLIC
        StructuresToolFactory
        StructuresToolViewFactory
    PRIVATE
        StructuresManager
        StructuresTool
        StructureTreeModel
        StructureDefinitionFile
#   view/structures/modeltest.cpp
        StructureView
        StructuresToolView
        StructureViewItemDelegate
)

# structurestool.h cannot be installed as it pulls (too) many private uninstalled headers
# KF5 TODO: any tools/controllers and views should be created by the resp. factories
# since some time, no more directly.
# So on next ABI change depublish tools/controllers & their views
# For now keeping all installed, in case someone is using them still/already.
# structtoolview.h is not useful, but we keep installing the header just in case someone includes it

oktetakastencontrollers_add_sublibrary(${LIBFRAMESPRINT_REL_DIR}
    PRIVATE
        PrintInfo
        AbstractFrameRenderer
        FramesToPaperPrinter
        HeaderFooterFrameRenderer
)

oktetakastencontrollers_add_sublibrary(view/print
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

oktetakastencontrollers_add_sublibrary(view/viewconfig
    PUBLIC
        ViewConfigControllerFactory
    PRIVATE
        BytesPerLineDialog
        BytesPerGroupDialog
        ViewConfigController
)

oktetakastencontrollers_add_sublibrary(view/viewstatus
    PUBLIC
        ViewStatusControllerFactory
    PRIVATE
        ViewStatusController
)

oktetakastencontrollers_add_sublibrary(view/viewmode
    PUBLIC
        ViewModeControllerFactory
    PRIVATE
        ViewModeController
)

oktetakastencontrollers_add_sublibrary(view/viewprofiles
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

oktetakastencontrollers_add_sublibrary(view/viewcontextmenu
    PUBLIC
        ViewContextMenuControllerFactory
    PRIVATE
        ViewContextMenuController
)

oktetakastencontrollers_add_sublibrary(utils
    PRIVATE
        LabelledToolBarWidget
        LabelledWidgetsToolBar
)

# create a static library for the unit tests. This means we don't have to compile the sources twice

add_library(poddecoder-typeeditors STATIC ${oktetakastencontrollers_typeeditor_LIB_SRCS})
set_target_properties( poddecoder-typeeditors PROPERTIES POSITION_INDEPENDENT_CODE TRUE )
target_link_libraries( poddecoder-typeeditors
PUBLIC
  OktetaGui
PRIVATE
  KF5::Completion
)

add_library( structuresdatatypes STATIC ${oktetakastencontrollers_structuresdatatypes_LIB_SRCS} )
set_target_properties( structuresdatatypes PROPERTIES POSITION_INDEPENDENT_CODE TRUE )
target_link_libraries( structuresdatatypes
PUBLIC
  OktetaCore
  KF5::ConfigGui
  Qt5::Script
PRIVATE
  poddecoder-typeeditors
  KF5::Completion
  KF5::ConfigWidgets
  KF5::I18n
  KF5::Service
)

list(APPEND oktetakastencontrollers_LIB_HDRS
    ${CMAKE_CURRENT_BINARY_DIR}/config-qca2.hpp
)

set(OPTIONAL_PRIVATE)
if( Qca-qt5_FOUND )
    set(OPTIONAL_PRIVATE qca-qt5)
endif()


okteta_add_library(Controllers NAMESPACE Okteta Kasten
    PUBLIC
        OktetaKastenGui
        OktetaKastenCore
        KastenGui
        KastenCore
    PRIVATE
        structuresdatatypes
        KF5::NewStuff
        KF5::I18n
        KF5::ConfigWidgets
        KF5::KCMUtils
        KF5::IconThemes
        KF5::Completion
        Qt5::PrintSupport
        Qt5::Script
        ${OPTIONAL_PRIVATE}
    VERSION     ${OKTETAKASTENCONTROLLERS_VERSION}
    SOVERSION   ${OKTETAKASTENCONTROLLERS_SO_VERSION}
    ABIVERSION  ${OKTETAKASTEN_ABI_VERSION} ${KASTEN_ABI_VERSION}
    SOURCES   ${oktetakastencontrollers_LIB_SRCS}
    HEADERS   ${oktetakastencontrollers_LIB_HDRS}
    CCHEADERS ${oktetakastencontrollers_LIB_CCHDRS}
    NO_VERSIONED_INCLUDEDIR
    NO_VERSIONED_PACKAGE_NAME
    REVERSE_NAMESPACE_INCLUDEDIR
    REVERSE_NAMESPACE_LIB
    NO_TARGET_NAMESPACE
)

target_include_directories(OktetaKastenControllers
    PRIVATE
        view/libbytearraychecksum
        view/libbytearrayfilter
        view/libbytearraysearch
        view/libfinddialog
        libconfigentries
        utils
        ${LIBFRAMESPRINT_REL_DIR}
)

if(NOT KF5_VERSION VERSION_LESS "5.57.0")
install(FILES view/structures/kns/okteta-structures.knsrc  DESTINATION  ${KDE_INSTALL_KNSRCDIR})
else()
install( FILES view/structures/kns/okteta-structures.knsrc  DESTINATION  ${KDE_INSTALL_CONFDIR} )
endif()

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
)


if( KASTEN_BUILD_INTERNAL_TEMPLATES )

okteta_add_sublibrary(ByteArrayFilterTemplates
    SUBDIR view/libbytearrayfilter/filter/template
    PRIVATE
        Template_ByteArrayFilter
        Template_ByteArrayFilterParameterSet
        Template_ByteArrayFilterParameterSetEdit
)

add_library(ByteArrayFilterTemplates  STATIC ${bytearrayfiltertemplates_LIB_SRCS})
target_include_directories(ByteArrayFilterTemplates
    PRIVATE
        view/libbytearrayfilter
        view/libbytearrayfilter/filter
)
# Configure export macros to be setup as with a static lib
# as the source files are reused for static ByteArrayFilterTemplates
# Saves doing a separate export header.
target_compile_definitions(ByteArrayFilterTemplates
    PUBLIC
        OKTETAKASTENCONTROLLERS_STATIC_DEFINE
)
target_link_libraries(ByteArrayFilterTemplates
    PUBLIC
        OktetaCore
        Qt5::Widgets
    PRIVATE
        KF5::I18n
)

okteta_add_sublibrary(ByteArrayCheckSumTemplates
    SUBDIR view/libbytearraychecksum/algorithm/template
    PRIVATE
        Template_ByteArrayCheckSumAlgorithm
        Template_ByteArrayChecksumParameterSet
        Template_ByteArrayChecksumParameterSetEdit
)

add_library(ByteArrayCheckSumTemplates  STATIC ${bytearraychecksumtemplates_LIB_SRCS})
target_include_directories(ByteArrayCheckSumTemplates
    PRIVATE
        view/libbytearrayfilter
        view/libbytearrayfilter/filter
        view/libbytearraychecksum
        view/libbytearraychecksum/algorithm
)
# Configure export macros to be setup as with a static lib
# as the source files are reused for static ByteArrayCheckSumTemplates
# Saves doing a separate export header.
target_compile_definitions(ByteArrayCheckSumTemplates
    PUBLIC
        OKTETAKASTENCONTROLLERS_STATIC_DEFINE
)
target_link_libraries(ByteArrayCheckSumTemplates
    PUBLIC
        OktetaCore
        Qt5::Widgets
    PRIVATE
        KF5::I18n
)

endif()

if(BUILD_TESTING)
    add_subdirectory(view/poddecoder/tests)
    add_subdirectory(view/replace/tests)
    add_subdirectory(view/search/tests)
    add_subdirectory(view/structures/tests)
endif()
