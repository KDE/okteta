# the service
kde_module_LTLIBRARIES = libkbyteseditwidget.la

INCLUDES = -I$(srcdir)/../../lib $(all_includes)

noinst_HEADERS = kbyteseditwidget.h

libkbyteseditwidget_la_SOURCES = kbyteseditwidget.cpp

libkbyteseditwidget_la_LDFLAGS = $(all_libraries) $(KDE_PLUGIN) -module
libkbyteseditwidget_la_LIBADD = ../../lib/libkhexedit.la $(LIB_KPARTS) $(LIB_KIO)

METASOURCES = AUTO

service_DATA = kbyteseditwidget.desktop
servicedir = $(kde_servicesdir)
