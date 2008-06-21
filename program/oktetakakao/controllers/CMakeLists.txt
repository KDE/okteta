include_directories(
  ../../../core
  ../../../gui
  ../../../gui/libcolumnsview
  ../../libkakao/core/document
  ../../libkakao/core/system
  ../../libkakao/gui/view
  ../../libkakao/gui/system
  ../../libkakao/gui/controller
  ../document
  ../io
  ../view
  view/libbytearraylineedit
  view/libbytearrayfilter
  view/libfinddialog
  view/libframesprint
)

set( OVERWRITEONLYCONTROLLER_SRCS
  document/overwriteonly/overwriteonlycontroller.cpp
)
set( OVERWRITEMODECONTROLLER_SRCS
  view/overwritemode/overwritemodecontroller.cpp
)
set( INFOCONTROLLER_SRCS
  view/info/statistictablemodel.cpp
  view/info/infotool.cpp
  view/info/infoview.cpp
  view/info/infocontroller.cpp
)
set( STRINGSEXTRACTCONTROLLER_SRCS
  view/stringsextract/stringsextracttool.cpp
  view/stringsextract/containedstringtablemodel.cpp
  view/stringsextract/stringsextractview.cpp
  view/stringsextract/stringsextractcontroller.cpp
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
  view/poddecoder/kprimitivetypesview.cpp
  view/poddecoder/poddecodercontroller.cpp
)
set( BYTETABLECONTROLLER_SRCS
  view/bytetable/bytetablemodel.cpp
  view/bytetable/bytetabletool.cpp
  view/bytetable/bytetableview.cpp
  view/bytetable/bytetablecontroller.cpp
)
set( SEARCHCONTROLLER_SRCS
  view/search/ksearchdialog.cpp
  view/search/searchcontroller.cpp
)
set( REPLACECONTROLLER_SRCS
  view/replace/kreplacedialog.cpp
  view/replace/kreplaceprompt.cpp
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
  view/filter/filtertool.cpp
  view/filter/filterview.cpp
  view/filter/filtercontroller.cpp
)
set( GOTOOFFSETCONTROLLER_SRCS
  view/gotooffset/kgotooffsetdialog.cpp
  view/gotooffset/gotooffsetcontroller.cpp
)
set( BOOKMARKSCONTROLLER_SRCS
  view/bookmarks/bookmarkscontroller.cpp
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
  view/print/abstractcolumnrenderer.cpp
  view/print/abstractcolumnframerenderer.cpp
  view/print/offsetcolumnrenderer.cpp
  view/print/bordercolumnrenderer.cpp
  view/print/bytecolumnrenderer.cpp
  view/print/valuecolumnrenderer.cpp
  view/print/charcolumnrenderer.cpp
  view/print/bytearrayframerenderer.cpp
)
set( VIEWCONFIGCONTROLLER_SRCS
  view/viewconfig/viewconfigcontroller.cpp
)

set( OKTETA_KAKAO_CONTROLLERS_SRCS
#   ${OVERWRITEONLYCONTROLLER_SRCS}
  ${OVERWRITEMODECONTROLLER_SRCS}
  ${BYTEARRAYLINEEDIT_LIB_SRCS}
  ${PODDECODERCONTROLLER_SRCS}
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
  ${LIBFRAMESPRINT_SRCS}
  ${PRINTCONTROLLER_SRCS}
  ${BOOKMARKSCONTROLLER_SRCS}
  ${VIEWCONFIGCONTROLLER_SRCS}
)

kde4_add_library( okteta_kakaocontrollers STATIC ${OKTETA_KAKAO_CONTROLLERS_SRCS} )
