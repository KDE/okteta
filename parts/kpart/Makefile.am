INCLUDES = -I$(srcdir)/../../lib $(all_includes)

noinst_HEADERS = khepart.h khebrowserextension.h khepartfactory.h

METASOURCES = AUTO

kde_module_LTLIBRARIES = libkhexedit2part.la

# the Part's source, library search path, and link libraries
libkhexedit2part_la_SOURCES = khepart.cpp khebrowserextension.cpp khepartfactory.cpp

libkhexedit2part_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
libkhexedit2part_la_LIBADD = ../../lib/libkhexedit.la $(LIB_KPARTS) $(LIB_KIO)

partdesktopdir   = $(kde_servicesdir)
partdesktop_DATA = khexedit2part.desktop

# this is where the part's XML-GUI resource file goes
partrcdir   = $(kde_datadir)/khexedit2part
partrc_DATA = khexedit2partui.rc
