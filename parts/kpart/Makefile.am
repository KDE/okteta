kde_module_LTLIBRARIES = libkhexedit2part.la

INCLUDES = -I$(srcdir)/../../lib $(all_includes)

noinst_HEADERS = khepart.h khebrowserextension.h khepartfactory.h

METASOURCES = AUTO

libkhexedit2part_la_SOURCES = khepart.cpp khebrowserextension.cpp khepartfactory.cpp

libkhexedit2part_la_LDFLAGS = $(all_libraries) $(KDE_PLUGIN) -module -no-undefined
libkhexedit2part_la_LIBADD = ../../lib/libkhexeditcommon.la $(LIB_KPARTS) $(LIB_KIO)

partdesktop_DATA = khexedit2part.desktop
partdesktopdir   = $(kde_servicesdir)

partrc_DATA = khexedit2partui.rc
partrcdir   = $(kde_datadir)/khexedit2part

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/khexedit2part.pot
