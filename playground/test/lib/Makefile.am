LIB_KHEXEDIT = $(top_builddir)/../kdeutils/khexedit/lib/libkhexedit.la
LIB_KTEST = $(top_builddir)/kdeutil/kde/test/libkdetest.la

METASOURCES = AUTO

check_PROGRAMS = testsection

testsection_SOURCES = testsection.cpp
testsection_LDFLAGS = $(all_libraries) $(KDE_RPATH)
testsection_LDADD = $(LIB_KHEXEDIT) $(LIB_KTEST)

KDE_CXXFLAGS = $(USE_EXCEPTIONS)
INCLUDES = -I$(top_srcdir)/../kdeutils/khexedit/lib -I$(top_srcdir)/kdeutil $(all_includes)

TESTS = testsection
