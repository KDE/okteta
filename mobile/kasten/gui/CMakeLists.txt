add_definitions( -DKDE_DEFAULT_DEBUG_AREA=1804 )

set( KASTEN_REL_DIR ../../../libs/kasten )

include_directories(
  ${KASTEN_REL_DIR}/gui
  ${KASTEN_REL_DIR}/gui/controller
  ${KASTEN_REL_DIR}/gui/view
  ${KASTEN_REL_DIR}/core
  ${KASTEN_REL_DIR}/core/system
  ${KASTEN_REL_DIR}/core/io
  ${KASTEN_REL_DIR}/core/document
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

set( mobilekastengui_controller_SRCS
  controller/abstractactioncontroller.cpp
)
set( mobilekastengui_controller_HDRS
  controller/abstractactioncontroller.h
)

set( mobilekastengui_LIB_SRCS
  ${mobilekastengui_shell_SRCS}
  ${mobilekastengui_controller_SRCS}
)

set( mobilekastengui_LIB_HDRS
  mobilekastengui_export.h
  ${mobilekastengui_shell_HDRS}
  ${mobilekastengui_controller_HDRS}
)


kde4_add_library( kasten2mobilegui SHARED ${mobilekastengui_LIB_SRCS} )
target_link_libraries( kasten2mobilegui
  kasten2gui
  kasten2core
  ${KDE4_KDEUI_LIBS}
)
set_target_properties( kasten2mobilegui  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION} )

install( TARGETS kasten2mobilegui  ${INSTALL_TARGETS_DEFAULT_ARGS} )

# install( FILES ${mobilekastengui_LIB_HDRS}  DESTINATION  ${INCLUDE_INSTALL_DIR}/kasten  COMPONENT Devel )
