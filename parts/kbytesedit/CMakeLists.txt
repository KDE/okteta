kde4_header()

include_directories(${CMAKE_SOURCE_DIR}/khexedit/lib/  )


########### next target ###############

set(kbyteseditwidget_PART_SRCS kbyteseditwidget.cpp )

kde4_automoc(${kbyteseditwidget_PART_SRCS})

kde4_add_plugin(kbyteseditwidget WITH_PREFIX ${kbyteseditwidget_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kbyteseditwidget )

target_link_libraries(kbyteseditwidget  ${KDE4_KPARTS_LIBS} khexeditcommon kparts )

install_targets(${PLUGIN_INSTALL_DIR} kbyteseditwidget )


########### install files ###############

install_files( ${SERVICES_INSTALL_DIR} FILES kbyteseditwidget.desktop )

kde4_footer()



#original Makefile.am contents follow:

#kde_module_LTLIBRARIES = libkbyteseditwidget.la
#
#INCLUDES = -I$(srcdir)/../../lib $(all_includes)
#
#noinst_HEADERS = kbyteseditwidget.h
#
#METASOURCES = AUTO
#
#libkbyteseditwidget_la_SOURCES = kbyteseditwidget.cpp
#
#libkbyteseditwidget_la_LDFLAGS = $(all_libraries) $(KDE_PLUGIN) -module -avoid-version -no-undefined
#libkbyteseditwidget_la_LIBADD = ../../lib/libkhexeditcommon.la $(LIB_KPARTS)
#
#service_DATA = kbyteseditwidget.desktop
#servicedir = $(kde_servicesdir)
