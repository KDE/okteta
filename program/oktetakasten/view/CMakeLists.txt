include_directories(
  ../document
  ../../../libs/kasten/gui
  ../../../libs/kasten/gui/view
  ../../../libs/kasten/core
  ../../../libs/kasten/core/entity
  ../../../libs/kasten/core/document
  ../../../gui
  ../../../gui/libcolumnsview
  ../../../core
)

# if(KDE4_BUILD_TESTS)
#   add_subdirectory(tests)
# endif(KDE4_BUILD_TESTS)

set( OKTETA_KASTEN_GUI_SRCS
  bytearrayjanusview.cpp
  kbytearraydisplayfactory.cpp
  kbytearraydisplay.cpp
)

kde4_add_library( okteta_kastengui STATIC ${OKTETA_KASTEN_GUI_SRCS} )
