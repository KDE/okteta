add_definitions( -DKDE_DEFAULT_DEBUG_AREA=1804 )

set( KASTEN_REL_DIR ../../../../kasten )

include_directories(
  ${KASTEN_REL_DIR}/core
  ${KASTEN_REL_DIR}/core/document
  ${KASTEN_REL_DIR}/core/io
  ${KASTEN_REL_DIR}/core/system
#   view
#   io
#   system
  shell
#   controller
)


set( mobilekastengui_shell_SRCS
  shell/mobileshellwindow.cpp
)
set( mobilekastengui_shell_HDRS
  shell/mobileshellwindow.h
)


set( mobilekastengui_LIB_SRCS
  ${mobilekastengui_shell_SRCS}
)

set( mobilekastengui_LIB_HDRS
  mobilekastengui_export.h
  ${mobilekastengui_shell_HDRS}
)


kde4_add_library( mobilekastengui SHARED ${mobilekastengui_LIB_SRCS} )
target_link_libraries( mobilekastengui
  kastencore
  ${KDE4_KDEUI_LIBS}
)
set_target_properties( mobilekastengui  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS mobilekastengui  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install( FILES ${mobilekastengui_LIB_HDRS}  DESTINATION  ${INCLUDE_INSTALL_DIR}/kasten  COMPONENT Devel )
