
include_directories( ${CMAKE_SOURCE_DIR}/khexedit/lib/  )


########### next target ###############

set(khexedit2part_PART_SRCS khepart.cpp khebrowserextension.cpp khepartfactory.cpp )

kde4_automoc(${khexedit2part_PART_SRCS})

kde4_add_plugin(khexedit2part WITH_PREFIX ${khexedit2part_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} khexedit2part )

target_link_libraries(khexedit2part  khexeditcore khexeditgui ${KDE4_KPARTS_LIBS} )

install(TARGETS khexedit2part  DESTINATION ${PLUGIN_INSTALL_DIR})


########### install files ###############

install( FILES khexedit2part.desktop  DESTINATION ${SERVICES_INSTALL_DIR})
install( FILES khexedit2partui.rc  DESTINATION ${DATA_INSTALL_DIR}/khexedit2part)




#original Makefile.am contents follow:

#kde_module_LTLIBRARIES = libkhexedit2part.la
#
#INCLUDES = -I$(srcdir)/../../lib $(all_includes)
#
#noinst_HEADERS = khepart.h khebrowserextension.h khepartfactory.h
#
#METASOURCES = AUTO
#
#libkhexedit2part_la_SOURCES = khepart.cpp khebrowserextension.cpp khepartfactory.cpp
#
#libkhexedit2part_la_LDFLAGS = $(all_libraries) $(KDE_PLUGIN) -module -no-undefined
#libkhexedit2part_la_LIBADD = ../../lib/libkhexeditcommon.la $(LIB_KPARTS) $(LIB_KIO)
#
#partdesktop_DATA = khexedit2part.desktop
#partdesktopdir   = $(kde_servicesdir)
#
#partrc_DATA = khexedit2partui.rc
#partrcdir   = $(kde_datadir)/khexedit2part
#
#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/khexedit2part.pot
