include_directories(
  ../../../core
  ../../../gui
  ../../../gui/libcolumnsview
  ../../libkakao/core/entity
  ../../libkakao/core/document
  ../../libkakao/gui/view
  ../document
)

# if(KDE4_BUILD_TESTS)
#   add_subdirectory(tests)
# endif(KDE4_BUILD_TESTS)

set( OKTETA_KAKAO_GUI_SRCS
  kbytearraydisplayfactory.cpp
  kbytearraydisplay.cpp
)

kde4_add_library( okteta_kakaogui STATIC ${OKTETA_KAKAO_GUI_SRCS} )
