INCLUDES = -I$(top_srcdir)/../kdeutils/khexedit/lib $(all_includes)

noinst_HEADERS = khepart.h khepartfactory.h

METASOURCES = AUTO

kde_module_LTLIBRARIES = libkhexedit2part.la

# the Part's source, library search path, and link libraries
libkhexedit2part_la_SOURCES = khepart.cpp khepartfactory.cpp
libkhexedit2part_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries) $(KDE_RPATH)
libkhexedit2part_la_LIBADD = $(LIB_KPARTS) $(LIB_KFILE) -lkhexedit

partdesktopdir   = $(kde_servicesdir)
partdesktop_DATA = khexedit2part.desktop

# this is where the part's XML-GUI resource file goes
partrcdir   = $(kde_datadir)/khexedit2part
partrc_DATA = khexedit2partui.rc
