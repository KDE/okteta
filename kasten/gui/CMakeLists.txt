set( KASTEN_REL_DIR ../../libs/kasten )
set( OKTETA_REL_DIR ../.. )

include_directories(
  view
  ../core
  ../core/document
  ${KASTEN_REL_DIR}/gui
  ${KASTEN_REL_DIR}/gui/view
  ${KASTEN_REL_DIR}/gui/io
  ${KASTEN_REL_DIR}/core
  ${KASTEN_REL_DIR}/core/document
  ${KASTEN_REL_DIR}/core/entity
  ${KASTEN_REL_DIR}/core/io
  ${OKTETA_REL_DIR}/gui
  ${OKTETA_REL_DIR}/gui/libcolumnsview
  ${OKTETA_REL_DIR}/core
)

# if(KDE4_BUILD_TESTS)
#   add_subdirectory(tests)
# endif(KDE4_BUILD_TESTS)

set( okteta_textrenderer_SRCS
  io/bytearrayviewtextrenderer/abstractcolumntextrenderer.cpp
  io/bytearrayviewtextrenderer/offsetcolumntextrenderer.cpp
  io/bytearrayviewtextrenderer/bordercolumntextrenderer.cpp
  io/bytearrayviewtextrenderer/abstractbytearraycolumntextrenderer.cpp
  io/bytearrayviewtextrenderer/valuebytearraycolumntextrenderer.cpp
  io/bytearrayviewtextrenderer/charbytearraycolumntextrenderer.cpp
  io/bytearrayviewtextrenderer/bytearrayrowscolumntextrenderer.cpp
)

set( okteta_values_streamencoder_SRCS
  io/bytearrayvaluesstreamencoderconfigeditor.cpp
  io/bytearrayvaluesstreamencoderconfigeditorfactory.cpp
  io/bytearrayvaluestreamencoder.cpp
)

set( okteta_sourcecode_streamencoder_SRCS
  io/bytearraysourcecodestreamencoderconfigeditor.cpp
  io/bytearraysourcecodestreamencoderconfigeditorfactory.cpp
  io/bytearraysourcecodestreamencoder.cpp
)

set( okteta_kastengui_io_SRCS
  io/abstractbytearraystreamencoder.cpp
  io/bytearraytextstreamencoderpreview.cpp
  ${okteta_values_streamencoder_SRCS}
  io/bytearraytextstreamencoder.cpp
  ${okteta_sourcecode_streamencoder_SRCS}
  ${okteta_textrenderer_SRCS}
  io/bytearrayviewtextstreamencoder.cpp
)

set( okteta_kastengui_view_SRCS
  view/bytearrayjanusview.cpp
  view/kbytearraydisplayfactory.cpp
  view/kbytearraydisplay.cpp
)

set( okteta_kastengui_LIB_SRCS
  ${okteta_kastengui_view_SRCS}
  ${okteta_kastengui_io_SRCS}
)

set( okteta_kastengui_view_HDRS
  view/kbytearraydisplayfactory.h
  view/kbytearraydisplay.h
)

set( okteta_kastengui_io_HDRS
  io/bytearrayviewtextstreamencoder.h
  io/bytearraysourcecodestreamencoder.h
  io/bytearraytextstreamencoder.h
  io/bytearrayvaluestreamencoder.h
  io/bytearrayvaluesstreamencoderconfigeditor.h
  io/bytearraysourcecodestreamencoderconfigeditor.h
  io/bytearraysourcecodestreamencoderconfigeditorfactory.h
  io/bytearrayvaluesstreamencoderconfigeditorfactory.h
  io/abstractbytearraystreamencoder.h
  io/bytearraytextstreamencoderpreview.h
)

set( oktete_kastengui_LIB_HDRS
  oktetakastengui_export.h
  ${okteta_kastengui_view_HDRS}
  ${okteta_kastengui_io_HDRS}
)

kde4_add_library( oktetakastengui  SHARED ${okteta_kastengui_LIB_SRCS} )

target_link_libraries( oktetakastengui
  oktetakastencore
  kastengui
  kastencore
  oktetagui
  oktetacore
  ${KDE4_KDEUI_LIBS}
)
set_target_properties( oktetakastengui  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS oktetakastengui  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${oktete_kastengui_LIB_HDRS}  DESTINATION  ${INCLUDE_INSTALL_DIR}/kasten  COMPONENT Devel )
