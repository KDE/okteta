kde_module_LTLIBRARIES = libkbyteseditwidget.la

INCLUDES = -I$(srcdir)/../../lib $(all_includes)

noinst_HEADERS = kbyteseditwidget.h

METASOURCES = AUTO

libkbyteseditwidget_la_SOURCES = kbyteseditwidget.cpp

libkbyteseditwidget_la_LDFLAGS = $(all_libraries) $(KDE_PLUGIN) -module -avoid-version -no-undefined
libkbyteseditwidget_la_LIBADD = ../../lib/libkhexeditcommon.la $(LIB_KPARTS)

service_DATA = kbyteseditwidget.desktop
servicedir = $(kde_servicesdir)
