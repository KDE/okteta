LIB_KHEXEDIT = $(top_builddir)/../kdeutils/khexedit/lib/libkhexeditcommon.la
LIB_KTEST = $(top_builddir)/kdeutil/kde/test/libkdetest.la

METASOURCES = AUTO

check_PROGRAMS = testcoord testsection
#check_PROGRAMS = testcoord testsection testplainbuffer testfixedsizebuffer
#check_PROGRAMS = testplainbuffer testfixedsizebuffer

testcoord_SOURCES = testcoord.cpp
testcoord_LDFLAGS = $(all_libraries) $(KDE_RPATH)
testcoord_LDADD = $(LIB_KHEXEDIT) $(LIB_KTEST)

testsection_SOURCES = testsection.cpp
testsection_LDFLAGS = $(all_libraries) $(KDE_RPATH)
testsection_LDADD = $(LIB_KHEXEDIT) $(LIB_KTEST)

testfixedsizebuffer_SOURCES = testdatabufferif.cpp testfixedsizebufferdatabufferif.cpp testfixedsizebuffer.cpp
testfixedsizebuffer_LDFLAGS = $(all_libraries) $(KDE_RPATH)
testfixedsizebuffer_LDADD = $(LIB_KHEXEDIT) $(LIB_KTEST)

testplainbuffer_SOURCES = testdatabufferif.cpp testplainbufferdatabufferif.cpp testplainbuffer.cpp
testplainbuffer_LDFLAGS = $(all_libraries) $(KDE_RPATH)
testplainbuffer_LDADD = $(LIB_KHEXEDIT) $(LIB_KTEST)


KDE_CXXFLAGS = $(USE_EXCEPTIONS)
INCLUDES = -I$(top_srcdir)/../kdeutils/khexedit/lib -I$(top_srcdir)/kdeutil $(all_includes)

TESTS = testsection testcoord
