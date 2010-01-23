set( KASTEN_REL_DIR ../../libs/kasten )
set( OKTETA_REL_DIR ../.. )

include_directories(
  view
  io/streamencoder
  liboktetawidgets
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

set( oktetawidgets_LIB_SRCS
  liboktetawidgets/bytearrayvalidator.cpp
  liboktetawidgets/bytearraycombobox_p.cpp
  liboktetawidgets/bytearraycombobox.cpp
  liboktetawidgets/addressvalidator.cpp
  liboktetawidgets/addresscombobox_p.cpp
  liboktetawidgets/addresscombobox.cpp
)

set( okteta_values_streamencoder_SRCS
  io/streamencoder/values/bytearrayvaluesstreamencoderconfigeditor.cpp
  io/streamencoder/values/bytearrayvaluesstreamencoderconfigeditorfactory.cpp
  io/streamencoder/values/bytearrayvaluesstreamencoder.cpp
)

set( okteta_base32_streamencoder_SRCS
  io/streamencoder/base32/bytearraybase32streamencoderconfigeditor.cpp
  io/streamencoder/base32/bytearraybase32streamencoderconfigeditorfactory.cpp
  io/streamencoder/base32/bytearraybase32streamencoder.cpp
)

set( okteta_uuencoding_streamencoder_SRCS
  io/streamencoder/uuencoding/bytearrayuuencodingstreamencoderconfigeditor.cpp
  io/streamencoder/uuencoding/bytearrayuuencodingstreamencoderconfigeditorfactory.cpp
  io/streamencoder/uuencoding/bytearrayuuencodingstreamencoder.cpp
)

set( okteta_sourcecode_streamencoder_SRCS
  io/streamencoder/sourcecode/bytearraysourcecodestreamencoderconfigeditor.cpp
  io/streamencoder/sourcecode/bytearraysourcecodestreamencoderconfigeditorfactory.cpp
  io/streamencoder/sourcecode/bytearraysourcecodestreamencoder.cpp
)

set( okteta_viewtext_streamencoder_SRCS
  io/streamencoder/viewtext/abstractcolumntextrenderer.cpp
  io/streamencoder/viewtext/offsetcolumntextrenderer.cpp
  io/streamencoder/viewtext/bordercolumntextrenderer.cpp
  io/streamencoder/viewtext/abstractbytearraycolumntextrenderer.cpp
  io/streamencoder/viewtext/valuebytearraycolumntextrenderer.cpp
  io/streamencoder/viewtext/charbytearraycolumntextrenderer.cpp
  io/streamencoder/viewtext/bytearrayrowscolumntextrenderer.cpp
  io/streamencoder/viewtext/bytearrayviewtextstreamencoder.cpp
)

set( okteta_streamencoder_SRCS
  io/streamencoder/abstractbytearraystreamencoder.cpp
  io/streamencoder/bytearraytextstreamencoderpreview.cpp
  ${okteta_values_streamencoder_SRCS}
  io/streamencoder/chars/bytearraycharsstreamencoder.cpp
  io/streamencoder/base64/bytearraybase64streamencoder.cpp
  ${okteta_base32_streamencoder_SRCS}
  io/streamencoder/base85/bytearraybase85streamencoder.cpp
  ${okteta_uuencoding_streamencoder_SRCS}
  io/streamencoder/srec/bytearraysrecstreamencoder.cpp
  io/streamencoder/ihex/bytearrayihexstreamencoder.cpp
  ${okteta_sourcecode_streamencoder_SRCS}
  ${okteta_viewtext_streamencoder_SRCS}
  io/bytearraystreamencoderfactory.cpp
  io/bytearraystreamencoderconfigeditorfactoryfactory.cpp
)

set( okteta_pattern_datagenerator_SRCS
  io/generator/pattern/bytearraypatterngeneratorconfigeditor.cpp
  io/generator/pattern/bytearraypatterngeneratorconfigeditorfactory.cpp
  io/generator/pattern/bytearraypatterngenerator.cpp
)

set( okteta_randomdata_datagenerator_SRCS
  io/generator/randomdata/bytearrayrandomdatageneratorconfigeditor.cpp
  io/generator/randomdata/bytearrayrandomdatageneratorconfigeditorfactory.cpp
  io/generator/randomdata/bytearrayrandomdatagenerator.cpp
)

set( okteta_sequence_datagenerator_SRCS
  io/generator/sequence/bytearraysequencegenerator.cpp
)

set( okteta_datagenerator_SRCS
  ${okteta_randomdata_datagenerator_SRCS}
  ${okteta_pattern_datagenerator_SRCS}
  ${okteta_sequence_datagenerator_SRCS}
  io/bytearraydatageneratorfactory.cpp
  io/bytearraydatageneratorconfigeditorfactoryfactory.cpp
)

set( okteta_kastengui_io_SRCS
  ${okteta_streamencoder_SRCS}
  ${okteta_datagenerator_SRCS}
)

set( okteta_kastengui_view_SRCS
  view/bytearrayjanusview.cpp
  view/bytearrayviewfactory.cpp
  view/bytearrayview.cpp
)

set( okteta_kastengui_LIB_SRCS
  ${oktetawidgets_LIB_SRCS}
  ${okteta_kastengui_view_SRCS}
  ${okteta_kastengui_io_SRCS}
)

set( okteta_kastengui_view_HDRS
  view/bytearrayviewfactory.h
  view/bytearrayview.h
)

set( okteta_kastengui_io_HDRS
  io/streamencoder/viewtext/bytearrayviewtextstreamencoder.h
  io/streamencoder/sourcecode/bytearraysourcecodestreamencoder.h
  io/streamencoder/chars/bytearraycharsstreamencoder.h
  io/streamencoder/values/bytearrayvaluesstreamencoder.h
  io/streamencoder/values/bytearrayvaluesstreamencoderconfigeditor.h
  io/streamencoder/sourcecode/bytearraysourcecodestreamencoderconfigeditor.h
  io/streamencoder/sourcecode/bytearraysourcecodestreamencoderconfigeditorfactory.h
  io/streamencoder/values/bytearrayvaluesstreamencoderconfigeditorfactory.h
  io/streamencoder/abstractbytearraystreamencoder.h
  io/streamencoder/bytearraytextstreamencoderpreview.h
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
