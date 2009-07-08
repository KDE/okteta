include_directories(
  ../../../core
  ../../../gui
  ../../../gui/libcolumnsview
  ../../libkakao/core/entity
  ../../libkakao/core/document
  ../../libkakao/core/io
  ../../libkakao/core/system
  ../../libkakao/gui/view
  ../../libkakao/gui/system
  ../../libkakao/gui/controller
  ../document
  ../io
  ../view
  view/libbytearraylineedit
  view/libbytearraychecksum
  view/libbytearrayfilter
  view/libfinddialog
  view/libframesprint
)

if( QCA2_FOUND )
  include_directories( ${QCA2_INCLUDE_DIR} )
endif( QCA2_FOUND )

macro_bool_to_01( QCA2_FOUND  HAVE_QCA2 )
configure_file( config-qca2.h.cmake  ${CMAKE_CURRENT_BINARY_DIR}/config-qca2.h )

set( DOCUMENTINFOCONTROLLER_SRCS
  document/info/documentinfotool.cpp
  document/info/documentinfoview.cpp
  document/info/documentinfotoolview.cpp
)
set( OVERWRITEONLYCONTROLLER_SRCS
  document/overwriteonly/overwriteonlycontroller.cpp
)
set( OVERWRITEMODECONTROLLER_SRCS
  view/overwritemode/overwritemodecontroller.cpp
)
set( INFOCONTROLLER_SRCS
  view/info/createstatisticjob.cpp
  view/info/statistictablemodel.cpp
  view/info/infotool.cpp
  view/info/infoview.cpp
  view/info/infotoolview.cpp
)
set( STRINGSEXTRACTCONTROLLER_SRCS
  view/stringsextract/extractstringsjob.cpp
  view/stringsextract/stringsextracttool.cpp
  view/stringsextract/containedstringtablemodel.cpp
  view/stringsextract/stringsextractview.cpp
  view/stringsextract/stringsextracttoolview.cpp
)

if( QCA2_FOUND )
    set( QCA2CHECKSUM_SRCS
      view/libbytearraychecksum/algorithm/qca2bytearraychecksumalgorithm.cpp
    )
endif( QCA2_FOUND )

set( LIBCHECKSUM_SRCS
  view/libbytearraychecksum/abstractbytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/abstractbytearraychecksumparameterset.cpp
  view/libbytearraychecksum/abstractbytearraychecksumparametersetedit.cpp
  view/libbytearraychecksum/algorithm/nobytearraychecksumparameterset.cpp
  view/libbytearraychecksum/algorithm/nobytearraychecksumparametersetedit.cpp
  view/libbytearraychecksum/algorithm/crc32bytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/algorithm/adler32bytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/algorithm/modsum8bytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/algorithm/modsum16bytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/algorithm/modsum32bytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/algorithm/modsum64bytearraychecksumalgorithm.cpp
  view/libbytearraychecksum/algorithm/modsumbytearraychecksumparameterset.cpp
  view/libbytearraychecksum/algorithm/modsumbytearraychecksumparametersetedit.cpp
  ${QCA2CHECKSUM_SRCS}
)
set( CHECKSUMCONTROLLER_SRCS
  view/checksum/checksumcalculatejob.cpp
  view/checksum/checksumtool.cpp
  view/checksum/checksumview.cpp
  view/checksum/checksumtoolview.cpp
)
set( BYTEARRAYLINEEDIT_LIB_SRCS
  view/libbytearraylineedit/kbytearrayvalidator.cpp
  view/libbytearraylineedit/kbytearraylineedit.cpp
)
set( FINDDIALOG_LIB_SRCS
  view/libfinddialog/kabstractfinddialog.cpp
)
set( PODDECODERCONTROLLER_SRCS
  view/poddecoder/poddata.cpp
  view/poddecoder/poddecodertool.cpp
  view/poddecoder/podtablemodel.cpp
  view/poddecoder/podtableview.cpp
  view/poddecoder/poddecodertoolview.cpp
)
set( BYTETABLECONTROLLER_SRCS
  view/bytetable/bytetablemodel.cpp
  view/bytetable/bytetabletool.cpp
  view/bytetable/bytetableview.cpp
  view/bytetable/bytetabletoolview.cpp
)
set( SEARCHCONTROLLER_SRCS
  view/search/ksearchdialog.cpp
  view/search/searchjob.cpp
  view/search/searchtool.cpp
  view/search/searchcontroller.cpp
)
set( REPLACECONTROLLER_SRCS
  view/replace/kreplacedialog.cpp
  view/replace/kreplaceprompt.cpp
  view/replace/replacetool.cpp
  view/replace/replacecontroller.cpp
)
set( INSERTPATTERNCONTROLLER_SRCS
  view/insertpattern/insertpatterntool.cpp
  view/insertpattern/insertpatterndialog.cpp
  view/insertpattern/insertpatterncontroller.cpp
)
set( LIBFILTER_SRCS
  view/libbytearrayfilter/abstractbytearrayfilter.cpp
  view/libbytearrayfilter/abstractbytearrayfilterparameterset.cpp
  view/libbytearrayfilter/abstractbytearrayfilterparametersetedit.cpp
  view/libbytearrayfilter/filter/nobytearrayfilterparameterset.cpp
  view/libbytearrayfilter/filter/operandbytearrayfilterparameterset.cpp
  view/libbytearrayfilter/filter/reversebytearrayfilterparameterset.cpp
  view/libbytearrayfilter/filter/rotatebytearrayfilterparameterset.cpp
  view/libbytearrayfilter/filter/andbytearrayfilter.cpp
  view/libbytearrayfilter/filter/orbytearrayfilter.cpp
  view/libbytearrayfilter/filter/xorbytearrayfilter.cpp
  view/libbytearrayfilter/filter/invertbytearrayfilter.cpp
  view/libbytearrayfilter/filter/reversebytearrayfilter.cpp
  view/libbytearrayfilter/filter/rotatebytearrayfilter.cpp
  view/libbytearrayfilter/filter/shiftbytearrayfilter.cpp
  view/libbytearrayfilter/filter/nobytearrayfilterparametersetedit.cpp
  view/libbytearrayfilter/filter/operandbytearrayfilterparametersetedit.cpp
  view/libbytearrayfilter/filter/reversebytearrayfilterparametersetedit.cpp
  view/libbytearrayfilter/filter/rotatebytearrayfilterparametersetedit.cpp
)
set( FILTERCONTROLLER_SRCS
  view/filter/filterjob.cpp
  view/filter/filtertool.cpp
  view/filter/filterview.cpp
  view/filter/filtertoolview.cpp
)
set( GOTOOFFSETCONTROLLER_SRCS
  view/gotooffset/gotooffsettool.cpp
  view/gotooffset/gotooffsetview.cpp
  view/gotooffset/gotooffsettoolview.cpp
  view/gotooffset/gotooffsetcontroller.cpp
)
set( SELECTRANGECONTROLLER_SRCS
  view/selectrange/selectrangetool.cpp
  view/selectrange/selectrangeview.cpp
  view/selectrange/selectrangetoolview.cpp
  view/selectrange/selectrangecontroller.cpp
)
set( BOOKMARKSCONTROLLER_SRCS
  view/bookmarks/bookmarkeditpopup.cpp
  view/bookmarks/bookmarkscontroller.cpp
)
set( BOOKMARKSTOOL_SRCS
  view/bookmarks/bookmarklistmodel.cpp
  view/bookmarks/bookmarkstool.cpp
  view/bookmarks/bookmarksview.cpp
  view/bookmarks/bookmarkstoolview.cpp
)
set( LIBFRAMESPRINT_SRCS
  view/libframesprint/abstractframerenderer.cpp
  view/libframesprint/framestopaperprinter.cpp
  view/libframesprint/headerfooterframerenderer.cpp
)
set( PRINTCONTROLLER_SRCS
  view/print/printcontroller.cpp
  view/print/printtool.cpp
  view/print/printjob.cpp
  view/print/abstractcolumnframerenderer.cpp
  view/print/printcolumnstylist.cpp
  view/print/bytearrayframerenderer.cpp
)
set( VIEWCONFIGCONTROLLER_SRCS
  view/viewconfig/viewconfigcontroller.cpp
)
set( VIEWSTATUSCONTROLLER_SRCS
  view/viewstatus/viewstatuscontroller.cpp
)
set( VIEWMODECONTROLLER_SRCS
  view/viewmode/viewmodecontroller.cpp
)
set( OKTETA_KAKAO_CONTROLLERS_SRCS
#   ${OVERWRITEONLYCONTROLLER_SRCS}
  ${OVERWRITEMODECONTROLLER_SRCS}
  ${DOCUMENTINFOCONTROLLER_SRCS}
  ${BYTEARRAYLINEEDIT_LIB_SRCS}
  ${PODDECODERCONTROLLER_SRCS}
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
  ${INSERTPATTERNCONTROLLER_SRCS}
  ${GOTOOFFSETCONTROLLER_SRCS}
  ${SELECTRANGECONTROLLER_SRCS}
  ${LIBFRAMESPRINT_SRCS}
  ${PRINTCONTROLLER_SRCS}
  ${BOOKMARKSCONTROLLER_SRCS}
  ${BOOKMARKSTOOL_SRCS}
  ${VIEWCONFIGCONTROLLER_SRCS}
  ${VIEWSTATUSCONTROLLER_SRCS}
  ${VIEWMODECONTROLLER_SRCS}
)

kde4_add_library( okteta_kakaocontrollers STATIC ${OKTETA_KAKAO_CONTROLLERS_SRCS} )
