set( LIBFRAMESPRINT_REL_DIR ../../libs/framesprint )

ecm_setup_version( ${OKTETAKASTEN_LIB_VERSION}
    VARIABLE_PREFIX OKTETAKASTENCONTROLLERS
    SOVERSION ${OKTETAKASTEN_LIB_SOVERSION}
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenControllersConfigVersion.cmake"
)

add_definitions(-DTRANSLATION_DOMAIN=\"liboktetakasten\")

macro(OKTETAKASTENCONTROLLERS_GENERATE_HEADERS _SUBLIB _RELATIVE)
    ecm_generate_headers("${_SUBLIB}_CCHDRS"
        HEADER_NAMES ${ARGN}
        RELATIVE ${_RELATIVE}
        PREFIX Kasten/Okteta
        REQUIRED_HEADERS "${_SUBLIB}_HDRS"
    )
endmacro()

include_directories(
  view/libbytearraychecksum
  view/libbytearrayfilter
  view/libfinddialog
  ${LIBFRAMESPRINT_REL_DIR}
)

set(HAVE_QCA2 ${Qca-qt5_FOUND})
configure_file( config-qca2.h.cmake  ${CMAKE_CURRENT_BINARY_DIR}/config-qca2.h )

set( DOCUMENTINFOCONTROLLER_SRCS
  document/info/bytearraymodeliodevice.cpp
  document/info/documentinfotool.cpp
  document/info/documentinfoview.cpp
  document/info/documentinfotoolview.cpp
)
oktetakastencontrollers_generate_headers(documentinfocontroller document/info
  DocumentInfoTool
  DocumentInfoToolView
)
set( OVERWRITEONLYCONTROLLER_SRCS
  document/overwriteonly/overwriteonlycontroller.cpp
)
oktetakastencontrollers_generate_headers(overwriteonlycontroller document/overwriteonly
  OverwriteOnlyController
)
set( OVERWRITEMODECONTROLLER_SRCS
  view/overwritemode/overwritemodecontroller.cpp
)
oktetakastencontrollers_generate_headers(overwritemodecontroller view/overwritemode
  OverwriteModeController
)
set( INFOCONTROLLER_SRCS
  view/info/createstatisticjob.cpp
  view/info/statistictablemodel.cpp
  view/info/infotool.cpp
  view/info/infoview.cpp
  view/info/infotoolview.cpp
  view/info/infotoolfactory.cpp
  view/info/infotoolviewfactory.cpp
)
kconfig_add_kcfg_files( INFOCONTROLLER_SRCS view/info/infoviewsettings.kcfgc )
oktetakastencontrollers_generate_headers(infocontroller view/info
  InfoTool
  InfoToolView
  InfoToolFactory
  InfoToolViewFactory
)

set( STRINGSEXTRACTCONTROLLER_SRCS
  view/stringsextract/extractstringsjob.cpp
  view/stringsextract/stringsextracttool.cpp
  view/stringsextract/containedstringtablemodel.cpp
  view/stringsextract/stringsextractview.cpp
  view/stringsextract/stringsextracttoolview.cpp
  view/stringsextract/stringsextracttoolfactory.cpp
  view/stringsextract/stringsextracttoolviewfactory.cpp
)
oktetakastencontrollers_generate_headers(stringsextractcontroller view/stringsextract
  ContainedString
  StringsExtractTool
  StringsExtractToolView
  StringsExtractToolFactory
  StringsExtractToolViewFactory
)

if( Qca-qt5_FOUND )
    set( QCA2CHECKSUM_SRCS
      view/libbytearraychecksum/algorithm/qca2bytearraychecksumalgorithm.cpp
    )
endif( Qca-qt5_FOUND )
set( LIBCHECKSUM_SRCS
  view/libbytearraychecksum/abstractbytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/abstractbytearraychecksumparameterset.cpp
  view/libbytearraychecksum/abstractbytearraychecksumparametersetedit.cpp
  view/libbytearraychecksum/bytearraychecksumalgorithmfactory.cpp
  view/libbytearraychecksum/bytearraychecksumparameterseteditfactory.cpp
  view/libbytearraychecksum/algorithm/crc32bytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/algorithm/adler32bytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/algorithm/modsum8bytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/algorithm/modsum16bytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/algorithm/modsum32bytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/algorithm/modsum64bytearraychecksumalgorithm.cpp
## NEWCHECKSUM(start)
## Here add the names of your source files of your checksum algorithm,
## e.g.
##   view/libbytearraychecksum/algorithm/mybytearraychecksumalgorithm.cpp
## NEWCHECKSUM(end)
  view/libbytearraychecksum/algorithm/nobytearraychecksumparameterset.cpp
  view/libbytearraychecksum/algorithm/nobytearraychecksumparametersetedit.cpp
  view/libbytearraychecksum/algorithm/modsumbytearraychecksumparameterset.cpp
  view/libbytearraychecksum/algorithm/modsumbytearraychecksumparametersetedit.cpp
## NEWCHECKSUMPARAMETERSET(start)
## Here add the names of your source files of your checksum parameter set and
## of the widget to edit the checksum parameter set,
## e.g.
##   view/libbytearraychecksum/algorithm/mybytearraychecksumparameterset.cpp
##   view/libbytearraychecksum/algorithm/mybytearraychecksumparametersetedit.cpp
## NEWCHECKSUMPARAMETERSET(end)
  ${QCA2CHECKSUM_SRCS}
)
set( CHECKSUMCONTROLLER_SRCS
  view/checksum/checksumcalculatejob.cpp
  view/checksum/checksumlogging.cpp
  view/checksum/checksumtool.cpp
  view/checksum/checksumview.cpp
  view/checksum/checksumtoolview.cpp
  view/checksum/checksumtoolfactory.cpp
  view/checksum/checksumtoolviewfactory.cpp
)
oktetakastencontrollers_generate_headers(checksumcontroller view/checksum
  ChecksumTool
  ChecksumToolView
  ChecksumToolFactory
  ChecksumToolViewFactory
)

set( FINDDIALOG_LIB_SRCS
  view/libfinddialog/kabstractfinddialog.cpp
)
oktetakastencontrollers_generate_headers(FINDDIALOG_LIB view/libfinddialog
  KFindDirection
)

set( PODDECODERCONTROLLER_SRCS
  view/poddecoder/typecodecs/binary8codec.cpp
  view/poddecoder/typecodecs/octal8codec.cpp
  view/poddecoder/typecodecs/hexadecimal8codec.cpp
  view/poddecoder/typecodecs/uint8codec.cpp
  view/poddecoder/typecodecs/uint16codec.cpp
  view/poddecoder/typecodecs/uint32codec.cpp
  view/poddecoder/typecodecs/uint64codec.cpp
  view/poddecoder/typecodecs/sint8codec.cpp
  view/poddecoder/typecodecs/sint16codec.cpp
  view/poddecoder/typecodecs/sint32codec.cpp
  view/poddecoder/typecodecs/sint64codec.cpp
  view/poddecoder/typecodecs/float32codec.cpp
  view/poddecoder/typecodecs/float64codec.cpp
  view/poddecoder/typecodecs/char8codec.cpp
  view/poddecoder/typecodecs/utf8codec.cpp
  view/poddecoder/poddata.cpp
  view/poddecoder/poddecodertool.cpp
  view/poddecoder/podtablemodel.cpp
  view/poddecoder/poddelegate.cpp
  view/poddecoder/podtableview.cpp
  view/poddecoder/poddecodertoolview.cpp
  view/poddecoder/poddecodertoolfactory.cpp
  view/poddecoder/poddecodertoolviewfactory.cpp
)
oktetakastencontrollers_generate_headers(poddecodercontroller view/poddecoder
  PODData
  PODDecoderTool
  PODDecoderToolView
  PODDecoderToolFactory
  PODDecoderToolViewFactory
)

set( PODDECODER_TYPEEDITOR_SRCS
  view/poddecoder/typeeditors/abstractvalue8editor.cpp
  view/poddecoder/typeeditors/binary8editor.cpp
  view/poddecoder/typeeditors/octal8editor.cpp
  view/poddecoder/typeeditors/hexadecimal8editor.cpp
  view/poddecoder/typeeditors/uintspinbox.cpp
  view/poddecoder/typeeditors/sintspinbox.cpp
  view/poddecoder/typeeditors/float32editor.cpp
  view/poddecoder/typeeditors/float64editor.cpp
  view/poddecoder/typeeditors/char8editor.cpp
  view/poddecoder/typeeditors/utf8editor.cpp
)

set( BYTETABLECONTROLLER_SRCS
  view/bytetable/bytetablemodel.cpp
  view/bytetable/bytetabletool.cpp
  view/bytetable/bytetableview.cpp
  view/bytetable/bytetabletoolview.cpp
  view/bytetable/bytetabletoolfactory.cpp
  view/bytetable/bytetabletoolviewfactory.cpp
)
kconfig_add_kcfg_files( BYTETABLECONTROLLER_SRCS view/bytetable/bytetableviewsettings.kcfgc )
oktetakastencontrollers_generate_headers(bytetablecontroller view/bytetable
  ByteTableTool
  ByteTableToolView
  ByteTableToolFactory
  ByteTableToolViewFactory
)

set( SEARCHCONTROLLER_SRCS
  view/search/ksearchdialog.cpp
  view/search/searchjob.cpp
  view/search/searchtool.cpp
  view/search/searchcontroller.cpp
)
oktetakastencontrollers_generate_headers(searchcontroller view/search
  SearchController
  SearchUserQueryable
)

set( REPLACECONTROLLER_SRCS
  view/replace/kreplacedialog.cpp
  view/replace/kreplaceprompt.cpp
  view/replace/replacetool.cpp
  view/replace/replacecontroller.cpp
)
oktetakastencontrollers_generate_headers(replacecontroller view/replace
  ReplaceController
  ReplaceUserQueryable
)

set( LIBFILTER_SRCS
  view/libbytearrayfilter/abstractbytearrayfilter.cpp
  view/libbytearrayfilter/abstractbytearrayfilterparameterset.cpp
  view/libbytearrayfilter/abstractbytearrayfilterparametersetedit.cpp
  view/libbytearrayfilter/bytearrayfilterfactory.cpp
  view/libbytearrayfilter/bytearrayfilterparameterseteditfactory.cpp
  view/libbytearrayfilter/filter/nobytearrayfilterparameterset.cpp
  view/libbytearrayfilter/filter/operandbytearrayfilterparameterset.cpp
  view/libbytearrayfilter/filter/reversebytearrayfilterparameterset.cpp
  view/libbytearrayfilter/filter/rotatebytearrayfilterparameterset.cpp
## NEWFILTERPARAMETERSET(start)
## Here add the names of your source files of your filter parameter set,
## e.g.
##  view/libbytearrayfilter/filter/mybytearrayfilterparameterset.cpp
## NEWFILTERPARAMETERSET(end)
  view/libbytearrayfilter/filter/andbytearrayfilter.cpp
  view/libbytearrayfilter/filter/orbytearrayfilter.cpp
  view/libbytearrayfilter/filter/xorbytearrayfilter.cpp
  view/libbytearrayfilter/filter/invertbytearrayfilter.cpp
  view/libbytearrayfilter/filter/reversebytearrayfilter.cpp
  view/libbytearrayfilter/filter/rotatebytearrayfilter.cpp
  view/libbytearrayfilter/filter/shiftbytearrayfilter.cpp
## NEWFILTER(start)
## Here add the names of your source files of your filter,
## e.g.
##   view/libbytearrayfilter/filter/mybytearrayfilter.cpp
## NEWFILTER(end)
  view/libbytearrayfilter/filter/nobytearrayfilterparametersetedit.cpp
  view/libbytearrayfilter/filter/operandbytearrayfilterparametersetedit.cpp
  view/libbytearrayfilter/filter/reversebytearrayfilterparametersetedit.cpp
  view/libbytearrayfilter/filter/rotatebytearrayfilterparametersetedit.cpp
## NEWFILTERPARAMETERSET(start)
## Here add the names of your source files of your widget to edit the filter parameter set,
## e.g.
##   view/libbytearrayfilter/filter/mybytearrayfilterparametersetedit.cpp
## NEWFILTERPARAMETERSET(end)
)
set( FILTERCONTROLLER_SRCS
  view/filter/filterjob.cpp
  view/filter/filtertool.cpp
  view/filter/filterview.cpp
  view/filter/filtertoolview.cpp
  view/filter/filtertoolfactory.cpp
  view/filter/filtertoolviewfactory.cpp
)
oktetakastencontrollers_generate_headers(filtercontroller view/filter
  FilterTool
  FilterToolView
  FilterToolFactory
  FilterToolViewFactory
)

set( CHARSETCONVERSIONTOOL_SRCS
  view/charsetconversion/charsetconversionjob.cpp
  view/charsetconversion/charsetconversiontool.cpp
  view/charsetconversion/charsetconversionview.cpp
  view/charsetconversion/charsetconversiontoolview.cpp
  view/charsetconversion/charsetconversiontoolfactory.cpp
  view/charsetconversion/charsetconversiontoolviewfactory.cpp
)
oktetakastencontrollers_generate_headers(charsetconversiontool view/charsetconversion
  CharsetConversionTool
  CharsetConversionToolView
  CharsetConversionToolFactory
  CharsetConversionToolViewFactory
)

set( GOTOOFFSETCONTROLLER_SRCS
  view/gotooffset/gotooffsettool.cpp
  view/gotooffset/gotooffsetview.cpp
  view/gotooffset/gotooffsettoolview.cpp
  view/gotooffset/gotooffsetcontroller.cpp
)
oktetakastencontrollers_generate_headers(gotooffsetcontroller view/gotooffset
  GotoOffsetController
)

set( SELECTRANGECONTROLLER_SRCS
  view/selectrange/selectrangetool.cpp
  view/selectrange/selectrangeview.cpp
  view/selectrange/selectrangetoolview.cpp
  view/selectrange/selectrangecontroller.cpp
)
oktetakastencontrollers_generate_headers(selectrangecontroller view/selectrange
  SelectRangeController
)

set( BOOKMARKSCONTROLLER_SRCS
  view/bookmarks/bookmarkeditpopup.cpp
  view/bookmarks/bookmarkscontroller.cpp
)
oktetakastencontrollers_generate_headers(bookmarkscontroller view/bookmarks
  BookmarksController
)

set( BOOKMARKSTOOL_SRCS
  view/bookmarks/bookmarklistmodel.cpp
  view/bookmarks/bookmarkstool.cpp
  view/bookmarks/bookmarksview.cpp
  view/bookmarks/bookmarkstoolview.cpp
  view/bookmarks/bookmarkstoolfactory.cpp
  view/bookmarks/bookmarkstoolviewfactory.cpp
)
oktetakastencontrollers_generate_headers(bookmarkstool view/bookmarks
  BookmarksTool
  BookmarksToolView
  BookmarksToolFactory
  BookmarksToolViewFactory
)

set( STRUCTVIEW_TESTED_SRCS
  view/structures/allprimitivetypes.cpp
  view/structures/datatypes/topleveldatainformation.cpp
  view/structures/datatypes/datainformation.cpp
  view/structures/datatypes/datainformationbase.cpp
  view/structures/datatypes/dummydatainformation.cpp
  view/structures/datatypes/datainformationwithchildren.cpp
  view/structures/datatypes/structuredatainformation.cpp
  view/structures/datatypes/taggeduniondatainformation.cpp
  view/structures/datatypes/uniondatainformation.cpp
  view/structures/datatypes/array/arraydatainformation.cpp
  view/structures/datatypes/array/abstractarraydata.cpp
  view/structures/datatypes/array/complexarraydata.cpp
  view/structures/datatypes/array/primitivearraydata.cpp
  view/structures/datatypes/primitive/pointerdatainformation.cpp
  view/structures/datatypes/primitive/chardatainformation.cpp
  view/structures/datatypes/primitive/doubledatainformation.cpp
  view/structures/datatypes/primitive/floatdatainformation.cpp
  view/structures/datatypes/primitive/sintdatainformation.cpp
  view/structures/datatypes/primitive/uintdatainformation.cpp
  view/structures/datatypes/primitive/booldatainformation.cpp
  view/structures/datatypes/primitive/flagdatainformation.cpp
  view/structures/datatypes/primitive/primitivedatainformation.cpp
  view/structures/datatypes/primitive/basicprimitivedatainformation.cpp
  view/structures/datatypes/primitive/bitfield/abstractbitfielddatainformation.cpp
  view/structures/datatypes/primitive/bitfield/boolbitfielddatainformation.cpp
  view/structures/datatypes/primitive/bitfield/signedbitfielddatainformation.cpp
  view/structures/datatypes/primitive/bitfield/unsignedbitfielddatainformation.cpp
  view/structures/datatypes/primitive/enumdatainformation.cpp
  view/structures/datatypes/primitive/enumdefinition.cpp
  view/structures/datatypes/primitivefactory.cpp
  view/structures/datatypes/primitivedatatype.cpp
  view/structures/datatypes/strings/stringdatainformation.cpp
  view/structures/datatypes/strings/stringdata.cpp
  view/structures/datatypes/strings/asciistringdata.cpp
  view/structures/datatypes/strings/latin1stringdata.cpp
  view/structures/datatypes/strings/utf8stringdata.cpp
  view/structures/datatypes/strings/utf16stringdata.cpp
  view/structures/datatypes/strings/utf32stringdata.cpp
  view/structures/script/scriptengineinitializer.cpp
  view/structures/script/scripthandler.cpp
  view/structures/script/scripthandlerinfo.cpp
  view/structures/script/scriptutils.cpp
  view/structures/script/scriptlogger.cpp
  view/structures/script/scriptloggerview.cpp
  view/structures/script/safereference.cpp
  view/structures/script/classes/defaultscriptclass.cpp
  view/structures/script/classes/arrayscriptclass.cpp
  view/structures/script/classes/primitivescriptclass.cpp
  view/structures/script/classes/enumscriptclass.cpp
  view/structures/script/classes/pointerscriptclass.cpp
  view/structures/script/classes/bitfieldscriptclass.cpp
  view/structures/script/classes/stringscriptclass.cpp
  view/structures/script/classes/structunionscriptclass.cpp
  view/structures/parsers/abstractstructureparser.cpp
  view/structures/parsers/osdparser.cpp
  view/structures/parsers/scriptfileparser.cpp
  view/structures/parsers/scriptvalueconverter.cpp
  view/structures/parsers/scriptvalueconverter_p.cpp
  view/structures/parsers/datainformationfactory.cpp
  view/structures/parsers/parserutils.cpp
  view/structures/structlogging.cpp
)

#these source files are not needed by the static library for unit tests, therefore keep them separate
set( STRUCTVIEW_OTHER_SRCS
  view/structures/structurestoolfactory.cpp
  view/structures/structurestoolviewfactory.cpp
  view/structures/structuresmanager.cpp
  view/structures/structtool.cpp
  view/structures/structtreemodel.cpp
  view/structures/structuredefinitionfile.cpp
#   view/structures/modeltest.cpp
  view/structures/structview.cpp
  view/structures/structtoolview.cpp
  view/structures/structviewitemdelegate.cpp
  view/structures/settings/structureaddremovewidget.cpp
  view/structures/settings/structviewdisplaysettingswidget.cpp
  view/structures/settings/structuresmanagerview.cpp
)

kconfig_add_kcfg_files( STRUCTVIEW_TESTED_SRCS view/structures/settings/structviewpreferences.kcfgc )
ki18n_wrap_ui( STRUCTVIEW_OTHER_SRCS view/structures/settings/structviewdisplaysettingswidget.ui )
# structtool.h cannot be installed as it pulls (too) many private uninstalled headers
# KF5 TODO: any tools/controllers and views should be created by the resp. factories
# since some time, no more directly.
# So on next ABI change depublish tools/controllers & their views
# For now keeping all installed, in case someone is using them still/already.
oktetakastencontrollers_generate_headers(structview_other view/structures
#   StructTool
  StructToolView
  StructuresToolFactory
  StructuresToolViewFactory
)

set( LIBFRAMESPRINT_SRCS
  ${LIBFRAMESPRINT_REL_DIR}/abstractframerenderer.cpp
  ${LIBFRAMESPRINT_REL_DIR}/framestopaperprinter.cpp
  ${LIBFRAMESPRINT_REL_DIR}/headerfooterframerenderer.cpp
)
set( PRINTCONTROLLER_SRCS
  ${LIBFRAMESPRINT_SRCS}
  view/print/printcontroller.cpp
  view/print/printtool.cpp
  view/print/printjob.cpp
  view/print/abstractcolumnframerenderer.cpp
  view/print/printcolumnstylist.cpp
  view/print/bytearrayframerenderer.cpp
)
oktetakastencontrollers_generate_headers(printcontroller view/print
  PrintController
)

set( VIEWCONFIGCONTROLLER_SRCS
  view/viewconfig/bytesperlinedialog.cpp
  view/viewconfig/bytespergroupdialog.cpp
  view/viewconfig/viewconfigcontroller.cpp
)
oktetakastencontrollers_generate_headers(viewconfigcontroller view/viewconfig
  ViewConfigController
)

set( VIEWSTATUSCONTROLLER_SRCS
  view/viewstatus/viewstatuscontroller.cpp
)
oktetakastencontrollers_generate_headers(viewstatuscontroller view/viewstatus
  ViewStatusController
)

set( VIEWMODECONTROLLER_SRCS
  view/viewmode/viewmodecontroller.cpp
)
oktetakastencontrollers_generate_headers(viewmodecontroller view/viewmode
  ViewModeController
)

set( VIEWPROFILESCONTROLLER_SRCS
  view/viewprofiles/viewprofileedit.cpp
  view/viewprofiles/viewprofileeditdialog.cpp
  view/viewprofiles/viewprofiletablemodel.cpp
  view/viewprofiles/viewprofilecontroller.cpp
)
oktetakastencontrollers_generate_headers(viewprofilescontroller view/viewprofiles
  ViewProfileController
)

set( VIEWPROFILESMANAGECONTROLLER_SRCS
  view/viewprofiles/viewprofilesmanagedialog.cpp
  view/viewprofiles/viewprofilesmanagecontroller.cpp
)
oktetakastencontrollers_generate_headers(viewprofilesmanagecontroller view/viewprofiles
  ViewProfilesManageController
)

# create a static library for the unit tests. This means we don't have to compile the sources twice

add_library( poddecoder-typeeditors STATIC ${PODDECODER_TYPEEDITOR_SRCS} )
set_target_properties( poddecoder-typeeditors PROPERTIES POSITION_INDEPENDENT_CODE TRUE )
target_link_libraries( poddecoder-typeeditors
PRIVATE
  OktetaGui
  OktetaCore
  KF5::Completion
)

add_library( structview-tested-srcs STATIC ${STRUCTVIEW_TESTED_SRCS} )
set_target_properties( structview-tested-srcs PROPERTIES POSITION_INDEPENDENT_CODE TRUE )
target_link_libraries( structview-tested-srcs
PRIVATE
  OktetaCore
  KF5::ConfigGui
  KF5::ConfigWidgets
  KF5::I18n
  KF5::Completion
  KF5::Service
  Qt5::Script
  Qt5::Xml
  Qt5::Widgets
)

set( OKTETA_KASTEN_CONTROLLERS_SRCS
#   ${OVERWRITEONLYCONTROLLER_SRCS}
  ${OVERWRITEMODECONTROLLER_SRCS}
  ${DOCUMENTINFOCONTROLLER_SRCS}
  ${LIBCHECKSUM_SRCS}
  ${CHECKSUMCONTROLLER_SRCS}
  ${STRINGSEXTRACTCONTROLLER_SRCS}
  ${BYTETABLECONTROLLER_SRCS}
  ${INFOCONTROLLER_SRCS}
  ${FINDDIALOG_LIB_SRCS}
  ${SEARCHCONTROLLER_SRCS}
  ${REPLACECONTROLLER_SRCS}
  ${LIBFILTER_SRCS}
  ${FILTERCONTROLLER_SRCS}
  ${CHARSETCONVERSIONTOOL_SRCS}
  ${GOTOOFFSETCONTROLLER_SRCS}
  ${SELECTRANGECONTROLLER_SRCS}
  ${PRINTCONTROLLER_SRCS}
  ${BOOKMARKSCONTROLLER_SRCS}
  ${BOOKMARKSTOOL_SRCS}
  ${VIEWCONFIGCONTROLLER_SRCS}
  ${VIEWSTATUSCONTROLLER_SRCS}
  ${VIEWMODECONTROLLER_SRCS}
  ${VIEWPROFILESCONTROLLER_SRCS}
  ${VIEWPROFILESMANAGECONTROLLER_SRCS}
  ${PODDECODERCONTROLLER_SRCS}
  ${STRUCTVIEW_OTHER_SRCS}
)


if (Qt5Test_FOUND)
  #structures view unit tests
  include(ECMAddTests)
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
    LINK_LIBRARIES structview-tested-srcs Qt5::Test poddecoder-typeeditors
  )
endif()

set( okteta_kastencontrollers_LIB_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/oktetakastencontrollers_export.h
  ${CMAKE_CURRENT_BINARY_DIR}/config-qca2.h
#   ${overwriteonlycontroller_HDRS}
  ${overwritemodecontroller_HDRS}
  ${documentinfocontroller_HDRS}
  ${libchecksum_HDRS}
  ${checksumcontroller_HDRS}
  ${stringsextractcontroller_HDRS}
  ${bytetablecontroller_HDRS}
  ${infocontroller_HDRS}
  ${finddialog_lib_HDRS}
  ${searchcontroller_HDRS}
  ${replacecontroller_HDRS}
  ${libfilter_HDRS}
  ${filtercontroller_HDRS}
  ${charsetconversiontool_HDRS}
  ${gotooffsetcontroller_HDRS}
  ${selectrangecontroller_HDRS}
  ${printcontroller_HDRS}
  ${bookmarkscontroller_HDRS}
  ${bookmarkstool_HDRS}
  ${viewconfigcontroller_HDRS}
  ${viewstatuscontroller_HDRS}
  ${viewmodecontroller_HDRS}
  ${viewprofilescontroller_HDRS}
  ${viewprofilesmanagecontroller_HDRS}
  ${poddecodercontroller_HDRS}
  ${structview_other_HDRS}
)
set( okteta_kastencontrollers_LIB_CCHDRS
#   ${overwriteonlycontroller_CCHDRS}
  ${overwritemodecontroller_CCHDRS}
  ${documentinfocontroller_CCHDRS}
  ${libchecksum_CCHDRS}
  ${checksumcontroller_CCHDRS}
  ${stringsextractcontroller_CCHDRS}
  ${bytetablecontroller_CCHDRS}
  ${infocontroller_CCHDRS}
  ${finddialog_lib_CCHDRS}
  ${searchcontroller_CCHDRS}
  ${replacecontroller_CCHDRS}
  ${libfilter_CCHDRS}
  ${filtercontroller_CCHDRS}
  ${charsetconversiontool_CCHDRS}
  ${gotooffsetcontroller_CCHDRS}
  ${selectrangecontroller_CCHDRS}
  ${printcontroller_CCHDRS}
  ${bookmarkscontroller_CCHDRS}
  ${bookmarkstool_CCHDRS}
  ${viewconfigcontroller_CCHDRS}
  ${viewstatuscontroller_CCHDRS}
  ${viewmodecontroller_CCHDRS}
  ${viewprofilescontroller_CCHDRS}
  ${viewprofilesmanagecontroller_CCHDRS}
  ${poddecodercontroller_CCHDRS}
  ${structview_other_CCHDRS}
)

add_library( OktetaKastenControllers  SHARED ${OKTETA_KASTEN_CONTROLLERS_SRCS} )
generate_export_header( OktetaKastenControllers )

target_link_libraries( OktetaKastenControllers
PUBLIC
  OktetaKastenGui
  OktetaKastenCore
  KastenGui
  KastenCore
PRIVATE
  structview-tested-srcs
  poddecoder-typeeditors
  KF5::NewStuff
  KF5::I18n
  KF5::ConfigWidgets
  KF5::KCMUtils
  KF5::IconThemes
  KF5::Completion
  Qt5::PrintSupport
  Qt5::Script
)
if( Qca-qt5_FOUND )
  target_link_libraries( OktetaKastenControllers
  PRIVATE
    qca-qt5
  )
endif( Qca-qt5_FOUND )

target_include_directories(OktetaKastenControllers PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/kasten/okteta>")

set_target_properties( OktetaKastenControllers  PROPERTIES
  EXPORT_NAME "OktetaKastenControllers"
  OUTPUT_NAME kasten${KASTEN_ABI_VERSION}okteta${OKTETAKASTEN_ABI_VERSION}controllers
  VERSION     ${OKTETAKASTENCONTROLLERS_VERSION}
  SOVERSION   ${OKTETAKASTENCONTROLLERS_SOVERSION}
)

install( TARGETS OktetaKastenControllers  EXPORT OktetaKastenControllersTargets ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${okteta_kastencontrollers_LIB_HDRS}
  DESTINATION ${OKTETAKASTEN_INCLUDE_INSTALL_DIR}
  COMPONENT Devel
)
install( FILES ${okteta_kastencontrollers_LIB_CCHDRS}
  DESTINATION ${OKTETAKASTEN_CCINCLUDE_INSTALL_DIR}
  COMPONENT Devel
)
install( FILES view/structures/kns/okteta-structures.knsrc  DESTINATION  ${CONFIG_INSTALL_DIR} )

set( elf_structure_FILES
  view/structures/examples/okteta/structures/elf/metadata.desktop
  view/structures/examples/okteta/structures/elf/main.osd
)
install( FILES ${elf_structure_FILES}  DESTINATION  ${DATA_INSTALL_DIR}/okteta/structures/elf )

set( png_structure_FILES
  view/structures/examples/okteta/structures/png/metadata.desktop
  view/structures/examples/okteta/structures/png/main.osd
)
install( FILES ${png_structure_FILES}  DESTINATION  ${DATA_INSTALL_DIR}/okteta/structures/png )
install( FILES view/structures/examples/okteta/structures/uuid.js  DESTINATION  ${DATA_INSTALL_DIR}/okteta/structures/ )
install( FILES view/structures/settings/structviewpreferences.kcfg  DESTINATION  ${KCFG_INSTALL_DIR} )

configure_file( view/structures/struct2osd.in ${CMAKE_CURRENT_BINARY_DIR}/struct2osd @ONLY )
install( PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/struct2osd DESTINATION ${BIN_INSTALL_DIR} )
install( FILES view/structures/gccxml-to-osd.xsl DESTINATION ${DATA_INSTALL_DIR}/okteta/structures/ )

# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/OktetaKasten")

ecm_configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/OktetaKastenControllersConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenControllersConfig.cmake"
  INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install( FILES
    "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenControllersConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/OktetaKastenControllersConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT OktetaKastenControllersTargets
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  FILE OktetaKastenControllersTargets.cmake
)


if( KASTEN_BUILD_INTERNAL_TEMPLATES )
include_directories(
  view/libbytearrayfilter/filter
  view/libbytearraychecksum/algorithm
)
set( libfilter_templates_SRCS
  view/libbytearrayfilter/filter/template/template_bytearrayfilter.cpp
  view/libbytearrayfilter/filter/template/template_bytearrayfilterparameterset.cpp
  view/libbytearrayfilter/filter/template/template_bytearrayfilterparametersetedit.cpp
)
set( libchecksum_templates_SRCS
  view/libbytearraychecksum/algorithm/template/template_bytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/algorithm/template/template_bytearraychecksumparameterset.cpp
  view/libbytearraychecksum/algorithm/template/template_bytearraychecksumparametersetedit.cpp
)

set( okteta_kastencontrollers_templates_SRCS
  ${libfilter_templates_SRCS}
  ${libchecksum_templates_SRCS}
)

add_library( oktetakastencontrollertemplates  STATIC ${okteta_kastencontrollers_templates_SRCS} )
# MSVC needs this
target_compile_definitions( oktetakastencontrollertemplates PUBLIC OKTETAKASTENCONTROLLERS_STATIC_DEFINE=1 )
target_link_libraries( oktetakastencontrollertemplates   KF5::I18n Qt5::Widgets OktetaCore )

endif( KASTEN_BUILD_INTERNAL_TEMPLATES )
