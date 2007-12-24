include_directories(
  ../../../core
  ../../../gui
  ../../../gui/libkcolumnsview
  ../../libkakao/core/document
  ../../libkakao/core/system
  ../../libkakao/gui/view
  ../../libkakao/gui/system
  ../../libkakao/gui/controller
  ../document
  ../io
  ../view
#   liblistview
  view/libbytearraylineedit
  view/libfinddialog
  view/libframesprint
#   chartable
)

set( LIBLISTVIEW_SRCS
  liblistview/listview.cpp
)
set( STATISTICCONTROLLER_SRCS
  document/statistic/fileinfodialog.cpp
  document/statistic/statisticcontroller.cpp
)
set( BYTEARRAYLINEEDIT_LIB_SRCS
  view/libbytearraylineedit/kbytearrayvalidator.cpp
  view/libbytearraylineedit/kbytearraylineedit.cpp
)
set( FINDDIALOG_LIB_SRCS
  view/libfinddialog/kabstractfinddialog.cpp
)
set( PODDECODERCONTROLLER_SRCS
  view/poddecoder/kprimitivetypesview.cpp
  view/poddecoder/poddecodercontroller.cpp
)
set( CHARTABLECONTROLLER_SRCS
  view/chartable/chartabledialog.cpp
  view/chartable/chartablecontroller.cpp
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
  view/insertpattern/kinsertpatterndialog.cpp
  view/insertpattern/insertpatterncontroller.cpp
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
#   ${LIBLISTVIEW_SRCS}
  ${BYTEARRAYLINEEDIT_LIB_SRCS}
  ${PODDECODERCONTROLLER_SRCS}
#   ${CHARTABLECONTROLLER_SRCS}
#   ${STATISTICCONTROLLER_SRCS}
  ${FINDDIALOG_LIB_SRCS}
  ${SEARCHCONTROLLER_SRCS}
  ${REPLACECONTROLLER_SRCS}
  ${INSERTPATTERNCONTROLLER_SRCS}
  ${GOTOOFFSETCONTROLLER_SRCS}
  ${LIBFRAMESPRINT_SRCS}
  ${PRINTCONTROLLER_SRCS}
  ${BOOKMARKSCONTROLLER_SRCS}
  ${VIEWCONFIGCONTROLLER_SRCS}
)

kde4_add_library( okteta_kakaocontrollers STATIC ${OKTETA_KAKAO_CONTROLLERS_SRCS} )
