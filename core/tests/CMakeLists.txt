set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

include_directories( ${SUTDIR} )

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions(-DOKTETACORE_EXPORT=)

# helper
set( oktetacoretestutil_LIB_SRCS
  util/fill.cpp
)

kde4_add_library( oktetacoretestutil  STATIC ${oktetacoretestutil_LIB_SRCS} )


########### arraychangemetricstest ###############

set( arraychangemetricstest_SRCS
  arraychangemetricstest.cpp
#   ${SUTDIR}/arraychangemetrics.cpp
)

kde4_add_unit_test( arraychangemetricstest  TESTNAME libokteta-core-arraychangemetricstest  ${arraychangemetricstest_SRCS} )
target_link_libraries( arraychangemetricstest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### sectiontest ###############

set( sectiontest_SRCS
  sectiontest.cpp
  ${SUTDIR}/section.cpp
)

kde4_add_unit_test( sectiontest  TESTNAME libokteta-core-sectiontest  ${sectiontest_SRCS} )
target_link_libraries( sectiontest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### sectionlisttest ###############

set( sectionlisttest_SRCS
  sectionlisttest.cpp
)

kde4_add_unit_test( sectionlisttest  TESTNAME libokteta-core-sectionlisttest  ${sectionlisttest_SRCS} )
target_link_libraries( sectionlisttest  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### fixedsizebytearraymodeltest ###############

set( fixedsizebytearraymodeltest_SRCS
  fixedsizebytearraymodeltest.cpp
)

kde4_add_unit_test( fixedsizebytearraymodeltest  TESTNAME libokteta-core-fixedsizebytearraymodeltest  ${fixedsizebytearraymodeltest_SRCS})
target_link_libraries( fixedsizebytearraymodeltest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### fixedsizebytearraymodelabstractbytearraymodeliftest ###############

set( fixedsizebytearraymodelabstractbytearraymodeliftest_SRCS
  abstractbytearraymodeliftest.cpp
  fixedsizebytearraymodelabstractbytearraymodeliftest.cpp
)

kde4_add_unit_test( fixedsizebytearraymodelabstractbytearraymodeliftest
  TESTNAME libokteta-core-fixedsizebytearraymodelabstractbytearraymodeliftest
  ${fixedsizebytearraymodelabstractbytearraymodeliftest_SRCS}
)
target_link_libraries( fixedsizebytearraymodelabstractbytearraymodeliftest
  oktetacoretestutil testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### kbytearraymodeltest ###############

set( kbytearraymodeltest_SRCS
  kbytearraymodeltest.cpp
)

kde4_add_unit_test( kbytearraymodeltest  TESTNAME libokteta-core-kbytearraymodeltest  ${kbytearraymodeltest_SRCS} )
target_link_libraries( kbytearraymodeltest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### kbytearraymodelkabstractbytearraymodeliftest ###############

set( kbytearraymodelkabstractbytearraymodeliftest_SRCS
  abstractbytearraymodeliftest.cpp
  kbytearraymodelkabstractbytearraymodeliftest.cpp
)

kde4_add_unit_test( kbytearraymodelkabstractbytearraymodeliftest
  TESTNAME libokteta-core-kbytearraymodelkabstractbytearraymodeliftest
  ${kbytearraymodelkabstractbytearraymodeliftest_SRCS}
)
target_link_libraries( kbytearraymodelkabstractbytearraymodeliftest
  oktetacoretestutil testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### piecetablebytearraymodelabstractbytearraymodeliftest ###############

set( piecetablebytearraymodelabstractbytearraymodeliftest_SRCS
  abstractbytearraymodeliftest.cpp
  piecetablebytearraymodelabstractbytearraymodeliftest.cpp
)

kde4_add_unit_test( piecetablebytearraymodelabstractbytearraymodeliftest
  TESTNAME libokteta-core-piecetablebytearraymodelabstractbytearraymodeliftest
  ${piecetablebytearraymodelabstractbytearraymodeliftest_SRCS}
)
target_link_libraries( piecetablebytearraymodelabstractbytearraymodeliftest
  oktetacoretestutil testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### piecetablebytearraymodelversionableiftest ###############

set( piecetablebytearraymodelversionableiftest_SRCS
  versionableiftest.cpp
  piecetablebytearraymodelversionableiftest.cpp
)

kde4_add_unit_test( piecetablebytearraymodelversionableiftest
  TESTNAME libokteta-core-piecetablebytearraymodelversionableiftest
  ${piecetablebytearraymodelversionableiftest_SRCS}
)
target_link_libraries( piecetablebytearraymodelversionableiftest
  oktetacoretestutil testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### kcharcodectest ###############

set( kcharcodectest_SRCS
  kcharcodectest.cpp
)

kde4_add_unit_test( kcharcodectest  TESTNAME libokteta-core-kcharcodectest  ${kcharcodectest_SRCS})
target_link_libraries( kcharcodectest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### ktextcharcodectest ###############

set( ktextcharcodectest_SRCS
  ktextcharcodectest.cpp
)

kde4_add_unit_test( ktextcharcodectest  TESTNAME libokteta-core-ktextcharcodectest  ${ktextcharcodectest_SRCS} )
target_link_libraries( ktextcharcodectest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### ktextcharcodeckcharcodeciftest ###############

set( ktextcharcodeckcharcodeciftest_SRCS
  kcharcodeciftest.cpp
  ktextcharcodeckcharcodeciftest.cpp
)

kde4_add_unit_test( ktextcharcodeckcharcodeciftest TESTNAME libokteta-core-ktextcharcodeckcharcodeciftest  ${ktextcharcodeckcharcodeciftest_SRCS} )
target_link_libraries( ktextcharcodeckcharcodeciftest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### kebcdic1047charcodeckcharcodeciftest ###############

set( kebcdic1047charcodeckcharcodeciftest_SRCS
  kcharcodeciftest.cpp kebcdic1047charcodeckcharcodeciftest.cpp
)

kde4_add_unit_test( kebcdic1047charcodeckcharcodeciftest  TESTNAME libokteta-core-kebcdic1047charcodeckcharcodeciftest  ${kebcdic1047charcodeckcharcodeciftest_SRCS} )
target_link_libraries( kebcdic1047charcodeckcharcodeciftest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### bookmarktest ###############

set( bookmarktest_SRCS
  bookmarktest.cpp
)

kde4_add_unit_test( bookmarktest  TESTNAME libokteta-core-bookmarktest  ${bookmarktest_SRCS} )
target_link_libraries( bookmarktest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### bookmarklisttest ###############

set( bookmarklisttest_SRCS
  bookmarklisttest.cpp
  ${SUTDIR}/bookmarklist.cpp
)

kde4_add_unit_test( bookmarklisttest  TESTNAME libokteta-core-bookmarklisttest  ${bookmarklisttest_SRCS} )
target_link_libraries( bookmarklisttest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


