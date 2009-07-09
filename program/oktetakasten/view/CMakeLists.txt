include_directories(
  ../../../core
  ../../../gui
  ../../../gui/libcolumnsview
  ../../../libs/kasten/core
  ../../../libs/kasten/core/entity
  ../../../libs/kasten/core/document
  ../../../libs/kasten/gui
  ../../../libs/kasten/gui/view
  ../document
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
