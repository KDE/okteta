set(OKTETAKASTENCONTROLLERS_VERSION ${OKTETAKASTEN_LIB_VERSION})
set(OKTETAKASTENCONTROLLERS_SO_VERSION ${OKTETAKASTEN_LIB_SOVERSION})

set( LIBFRAMESPRINT_REL_DIR ../../libs/framesprint )

# create static libraries for sharing between unit tests and actual library product
# This means we don't have to compile the sources twice

add_library(datatypeeditors STATIC)
set_target_properties(datatypeeditors PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(datatypeeditors
    PUBLIC
        OktetaGui
    PRIVATE
        KF5::Completion
)

add_library(structureview-tested-srcs STATIC)
set_target_properties(structureview-tested-srcs PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
target_link_libraries(structureview-tested-srcs
    PUBLIC
        OktetaCore
        KF5::ConfigGui
        KF5::I18n
        KF5::Service
        Qt5::Script
        Qt5::Xml
        Qt5::Widgets
    PRIVATE
        datatypeeditors
        KF5::Completion
        KF5::ConfigWidgets
)

ecm_qt_declare_logging_category(structureview-tested-srcs
    HEADER "structureslogging.hpp"
    IDENTIFIER "LOG_KASTEN_OKTETA_CONTROLLERS_STRUCTURES"
    CATEGORY_NAME "kasten.okteta.controllers.structures"
    DESCRIPTION "Okteta Structures tool"
    EXPORT OKTETA
)

okteta_add_library(Controllers NAMESPACE Okteta Kasten
    PUBLIC
        OktetaKastenGui
        OktetaKastenCore
        KastenGui
        KastenCore
    PRIVATE
        structureview-tested-srcs
        datatypeeditors
        KF5::NewStuffWidgets
        KF5::I18n
        KF5::ConfigWidgets
        KF5::ItemViews
        KF5::Completion
        Qt5::PrintSupport
        Qt5::Script
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
)

target_compile_definitions(OktetaKastenControllers PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")

ecm_qt_declare_logging_category(OktetaKastenControllers
    HEADER "checksumlogging.hpp"
    IDENTIFIER "LOG_OKTETA_KASTEN_CONTROLLER_CHECKSUM"
    CATEGORY_NAME "kasten.okteta.controllers.checksum"
    DESCRIPTION "Okteta Checksum tool"
    EXPORT OKTETA
)

macro(OKTETAKASTENCONTROLLERS_ADD_SUBLIBRARY _subdir)
    okteta_add_sublibrary(Controllers NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()

macro(OKTETAKASTENCONTROLLERS_ADD_SUBLIBRARY_STRUCTURES_TESTED _subdir)
    okteta_add_sublibrary(Controllers NAMESPACE Okteta Kasten
        REVERSE_NAMESPACE_INCLUDEDIR
        LIBRARY structureview-tested-srcs
        SUBDIR ${_subdir}
        ${ARGN}
    )
endmacro()


oktetakastencontrollers_add_sublibrary(libconfigentries
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
    BUILD_INCLUDEDIR
    PRIVATE
        AbstractByteArrayChecksumAlgorithm
        AbstractByteArrayChecksumParameterSet
        AbstractByteArrayChecksumParameterSetEdit
        ByteArrayChecksumAlgorithmFactory
        ByteArrayChecksumParameterSetEditFactory
)

oktetakastencontrollers_add_sublibrary(view/libbytearraychecksum/algorithm
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

oktetakastencontrollers_add_sublibrary(view/checksum
    PUBLIC
        ChecksumToolFactory
        ChecksumToolViewFactory
    PRIVATE
        ChecksumCalculateJob
        ChecksumView
        ChecksumTool
        ChecksumToolView
)

oktetakastencontrollers_add_sublibrary(view/libfinddialog
    BUILD_INCLUDEDIR
    PRIVATE
        AbstractFindDialog
        FindDirection
)

oktetakastencontrollers_add_sublibrary(view/libbytearraysearch
    BUILD_INCLUDEDIR
    PRIVATE
        ByteArraySearchJob
        ByteArraySearchUtils
)

oktetakastencontrollers_add_sublibrary(view/libdatatypes
    BUILD_INCLUDEDIR
    LIBRARY datatypeeditors
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

oktetakastencontrollers_add_sublibrary(view/libdatatypedisplaystringifiers
    BUILD_INCLUDEDIR
    LIBRARY datatypeeditors
    PRIVATE
        AbstractDataTypeDisplayStringifier
        DataTypeDisplayStringifierManager
)

oktetakastencontrollers_add_sublibrary(view/libdatatypeeditors
    BUILD_INCLUDEDIR
    LIBRARY datatypeeditors
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

oktetakastencontrollers_add_sublibrary(view/libdatatypecodecs
    BUILD_INCLUDEDIR
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

oktetakastencontrollers_add_sublibrary(view/poddecoder
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

oktetakastencontrollers_add_sublibrary(view/crop
    PUBLIC
        CropControllerFactory
    PRIVATE
        CropController
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
    BUILD_INCLUDEDIR
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


oktetakastencontrollers_add_sublibrary_structures_tested(view/structures/datatypes
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

oktetakastencontrollers_add_sublibrary_structures_tested(view/structures/datatypes/array
    PRIVATE
        ArrayDataInformation
        AbstractArrayData
        ComplexArrayData
        PrimitiveArrayData
)

oktetakastencontrollers_add_sublibrary_structures_tested(view/structures/datatypes/primitive
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

oktetakastencontrollers_add_sublibrary_structures_tested(view/structures/datatypes/primitive/bitfield
    PRIVATE
        AbstractBitfieldDataInformation
        BoolBitfieldDataInformation
        SignedBitfieldDataInformation
        UnsignedBitfieldDataInformation
)

oktetakastencontrollers_add_sublibrary_structures_tested(view/structures/datatypes/strings
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
oktetakastencontrollers_add_sublibrary_structures_tested(../../core/charcodecs
    PRIVATE
        Ebcdic1047CharCodec
)

oktetakastencontrollers_add_sublibrary_structures_tested(view/structures/script
    PRIVATE
        ScriptEngineInitializer
        ScriptHandler
        ScriptHandlerInfo
        ScriptLogger
        ScriptLoggerView
        SafeReference
)

oktetakastencontrollers_add_sublibrary_structures_tested(view/structures/script/classes
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

oktetakastencontrollers_add_sublibrary_structures_tested(view/structures/parsers
    PRIVATE
        AbstractStructureParser
        OsdParser
        ScriptFileParser
        ScriptValueConverter
        DataInformationFactory
        ParserUtils
)

oktetakastencontrollers_add_sublibrary_structures_tested(view/structures/settings
    KCFG
        structureviewpreferences.kcfgc
)

oktetakastencontrollers_add_sublibrary_structures_tested(view/structures
    PRIVATE
        AllPrimitiveTypes
)

#these source files are not needed by the static library for unit tests, therefore keep them separate
oktetakastencontrollers_add_sublibrary(view/structures/settings
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
oktetakastencontrollers_add_sublibrary(view/structures
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

oktetakastencontrollers_add_sublibrary(${LIBFRAMESPRINT_REL_DIR}
    BUILD_INCLUDEDIR
    PRIVATE
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
set_source_files_properties(view/print/printtool.cpp PROPERTIES
    COMPILE_FLAGS -DOKTETA_VERSION=\\\"${Okteta_VERSION}\\\"
)


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
    BUILD_INCLUDEDIR
    PRIVATE
        LabelledToolBarWidget
)

if(BUILD_TESTING)
    # libdatatypeeditors unit tests
    ecm_add_test(
        view/libdatatypeeditors/char8stringvalidator.cpp
        view/libdatatypeeditors/char8stringparser.cpp
        test/char8stringvalidatortest.cpp

        TEST_NAME char8stringvalidatortest
        LINK_LIBRARIES
            OktetaCore
            Qt5::Gui
            Qt5::Test
    )
    target_include_directories(char8stringvalidatortest
        PUBLIC
            view/libdatatypeeditors
    )

    #structures view unit tests
    ecm_add_tests(
        test/primitivearraytest.cpp
        test/arraydatainformationtest.cpp
        test/basicdatainformationtest.cpp
        test/primitivedatainformationtest.cpp
        test/scriptclassestest.cpp
        test/uniondatainformationtest.cpp
        test/allprimitivetypestest.cpp
        test/scriptvalueconvertertest.cpp
        test/osdparsertest.cpp
        test/commonparsertest.cpp
        test/jsparsertest.cpp
        test/customtostringtest.cpp
        test/locktooffsettest.cpp

        NAME_PREFIX structures-
        LINK_LIBRARIES structureview-tested-srcs Qt5::Test
    )

    # remove useless default definition of QT_TESTCASE_BUILDDIR in interface from imported target Qt5::Test
    get_target_property(qttestdefs Qt5::Test INTERFACE_COMPILE_DEFINITIONS)
    list(FILTER qttestdefs EXCLUDE REGEX [[^QT_TESTCASE_BUILDDIR=]])
    set_property(TARGET Qt5::Test PROPERTY INTERFACE_COMPILE_DEFINITIONS ${qttestdefs})
    # set the definition as needed for our tests
    target_compile_definitions(customtostringtest PRIVATE QT_TESTCASE_BUILDDIR="${CMAKE_CURRENT_BINARY_DIR}")
    target_compile_definitions(jsparsertest       PRIVATE QT_TESTCASE_BUILDDIR="${CMAKE_CURRENT_BINARY_DIR}")
    # TODO: still fragile once __FILE__ is neither absolute or full relative path to this build dir
    # look into using custom code intead of simple QFINDTESTDATA usage

    # search tool unit tests
    ecm_add_test(
        view/libbytearraysearch/bytearraysearchutils.cpp
        view/libbytearraysearch/bytearraysearchjob.cpp
        view/search/searchtool.cpp
        test/searchtooltest.cpp

        TEST_NAME searchtooltest
        LINK_LIBRARIES
            OktetaKastenGui
            OktetaKastenCore
            KF5::I18n
            Qt5::Test
    )
    target_include_directories(searchtooltest
        PUBLIC
            libconfigentries
            view/libfinddialog
            view/libbytearraysearch
    )

    #replace tool unit tests
    ecm_add_test(
        view/libbytearraysearch/bytearraysearchjob.cpp
        view/replace/replacejob.cpp
        test/replacejobtest.cpp

        TEST_NAME replacejobtest
        LINK_LIBRARIES
            OktetaKastenGui
            OktetaKastenCore
            Qt5::Test
    )
    target_include_directories(replacejobtest
        PUBLIC
            view/libfinddialog
            view/libbytearraysearch
    )
endif()

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

add_library(OktetaKastenControllersTemplates  STATIC)
okteta_add_sublibrary(ControllersTemplates NAMESPACE Okteta Kasten
    SUBDIR view/libbytearrayfilter/filter/template
    PRIVATE
        Template_ByteArrayFilter
        Template_ByteArrayFilterParameterSet
        Template_ByteArrayFilterParameterSetEdit
)
okteta_add_sublibrary(ControllersTemplates NAMESPACE Okteta Kasten
    SUBDIR view/libbytearraychecksum/algorithm/template
    PRIVATE
        Template_ByteArrayCheckSumAlgorithm
        Template_ByteArrayChecksumParameterSet
        Template_ByteArrayChecksumParameterSetEdit
)

target_include_directories(OktetaKastenControllersTemplates
    PRIVATE
        view/libbytearrayfilter
        view/libbytearrayfilter/filter
        view/libbytearraychecksum
        view/libbytearraychecksum/algorithm
)
# Configure export macros to be setup as with a static lib
# as the source files are reused for static OktetaKastenControllersTemplates
# Saves doing a separate export header.
target_compile_definitions(OktetaKastenControllersTemplates
    PUBLIC
        OKTETAKASTENCONTROLLERS_STATICLIB
)
target_link_libraries(OktetaKastenControllersTemplates
    PUBLIC
        OktetaCore
        Qt5::Widgets
    PRIVATE
        KF5::I18n
)

endif()
