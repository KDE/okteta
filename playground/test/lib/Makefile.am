LIB_KHEXEDIT = $(top_builddir)/../kdeutils/khexedit/lib/libkhexeditcommon.la
LIB_KTEST = $(top_builddir)/kdeutil/kde/test/libkdetest.la

METASOURCES = AUTO

check_PROGRAMS = testcoord testsection testplainbuffer testfixedsizebuffer \
                 testcharcodec testtextcharcodec testebcdic1047charcodec
#check_PROGRAMS = testcoord testsection testplainbuffer testfixedsizebuffer
#check_PROGRAMS = testplainbuffer testfixedsizebuffer

check_LTLIBRARIES =  libdatabufferif.la libcharcodecif.la

libdatabufferif_la_SOURCES = testdatabufferif.cpp

libcharcodecif_la_SOURCES = testcharcodecif.cpp

testcoord_SOURCES = testcoord.cpp
testcoord_LDFLAGS = $(all_libraries)
testcoord_LDADD = $(LIB_KHEXEDIT) $(LIB_KTEST)

testsection_SOURCES = testsection.cpp
testsection_LDFLAGS = $(all_libraries)
testsection_LDADD = $(LIB_KHEXEDIT) $(LIB_KTEST)

testcharcodec_SOURCES = testcharcodec.cpp
testcharcodec_LDFLAGS = $(all_libraries)
testcharcodec_LDADD = $(LIB_KHEXEDIT) $(LIB_KTEST)

testtextcharcodec_SOURCES = testtextcharcodeccharcodecif.cpp testtextcharcodec.cpp
testtextcharcodec_LDFLAGS = $(all_libraries)
testtextcharcodec_LDADD = $(LIB_KHEXEDIT) libcharcodecif.la $(LIB_KTEST)

testebcdic1047charcodec_SOURCES = testebcdic1047charcodeccharcodecif.cpp
testebcdic1047charcodec_LDFLAGS = $(all_libraries)
testebcdic1047charcodec_LDADD = $(LIB_KHEXEDIT) libcharcodecif.la $(LIB_KTEST)

testfixedsizebuffer_SOURCES = testfixedsizebufferdatabufferif.cpp testfixedsizebuffer.cpp
testfixedsizebuffer_LDFLAGS = $(all_libraries)
testfixedsizebuffer_LDADD = $(LIB_KHEXEDIT) libdatabufferif.la $(LIB_KTEST)

testplainbuffer_SOURCES = testplainbufferdatabufferif.cpp testplainbuffer.cpp
testplainbuffer_LDFLAGS = $(all_libraries)
testplainbuffer_LDADD = $(LIB_KHEXEDIT) libdatabufferif.la $(LIB_KTEST)


KDE_CXXFLAGS = $(USE_EXCEPTIONS)
INCLUDES = -I$(top_srcdir)/../kdeutils/khexedit/lib -I$(top_srcdir)/kdeutil $(all_includes)

TESTS = testsection testcoord testfixedsizebuffer testplainbuffer \
        testcharcodec testtextcharcodec testebcdic1047charcodec
