# set the include path for X, qt and KDE
INCLUDES = -I../lib $(all_includes)

# these are the headers for your project
noinst_HEADERS = khepart.h khepartfactory.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/khexedit.pot

#KDE_ICON = khexedit

kde_module_LTLIBRARIES = libkhexedit2part.la

# the Part's source, library search path, and link libraries
libkhexedit2part_la_SOURCES = khepart.cpp khepartfactory.cpp
libkhexedit2part_la_LDFLAGS = -module $(KDE_PLUGIN) $(all_libraries)
libkhexedit2part_la_LIBADD = ../lib/libkhexedit.la $(LIB_KPARTS) $(LIB_KFILE)

# this is where the desktop file will go
partdesktopdir   = $(kde_servicesdir)
partdesktop_DATA = khexedit2part.desktop

# this is where the part's XML-GUI resource file goes
partrcdir   = $(kde_datadir)/khexedit2
partrc_DATA = khexedit2partui.rc
