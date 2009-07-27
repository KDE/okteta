set( KASTEN_REL_DIR ../../libs/kasten )
set( OKTETA_REL_DIR ../.. )

include_directories(
  ../core/document
  ${KASTEN_REL_DIR}/gui
  ${KASTEN_REL_DIR}/gui/view
  ${KASTEN_REL_DIR}/core
  ${KASTEN_REL_DIR}/core/document
  ${KASTEN_REL_DIR}/core/entity
  ${OKTETA_REL_DIR}/gui
  ${OKTETA_REL_DIR}/gui/libcolumnsview
  ${OKTETA_REL_DIR}/core
)

# if(KDE4_BUILD_TESTS)
#   add_subdirectory(tests)
# endif(KDE4_BUILD_TESTS)

set( OKTETA_KASTEN_GUI_SRCS
  view/bytearrayjanusview.cpp
  view/kbytearraydisplayfactory.cpp
  view/kbytearraydisplay.cpp
)

kde4_add_library( okteta_kastengui STATIC ${OKTETA_KASTEN_GUI_SRCS} )
