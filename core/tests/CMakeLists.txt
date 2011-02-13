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


########### addressrangetest ###############

set( addressrangetest_SRCS
  addressrangetest.cpp
)

kde4_add_unit_test( addressrangetest  TESTNAME libokteta-core-addressrangetest  ${addressrangetest_SRCS} )
target_link_libraries( addressrangetest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### addressrangelisttest ###############

set( addressrangelisttest_SRCS
  addressrangelisttest.cpp
)

kde4_add_unit_test( addressrangelisttest  TESTNAME libokteta-core-addressrangelisttest  ${addressrangelisttest_SRCS} )
target_link_libraries( addressrangelisttest  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


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


########### bytearraymodeltest ###############

set( bytearraymodeltest_SRCS
  bytearraymodeltest.cpp
)

kde4_add_unit_test( bytearraymodeltest  TESTNAME libokteta-core-bytearraymodeltest  ${bytearraymodeltest_SRCS} )
target_link_libraries( bytearraymodeltest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### bytearraymodelabstractbytearraymodeliftest ###############

set( bytearraymodelabstractbytearraymodeliftest_SRCS
  abstractbytearraymodeliftest.cpp
  bytearraymodelabstractbytearraymodeliftest.cpp
)

kde4_add_unit_test( bytearraymodelabstractbytearraymodeliftest
  TESTNAME libokteta-core-bytearraymodelabstractbytearraymodeliftest
  ${bytearraymodelabstractbytearraymodeliftest_SRCS}
)
target_link_libraries( bytearraymodelabstractbytearraymodeliftest
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


########### valuecodectest ###############

set( valuecodectest_SRCS
  valuecodectest.cpp
)

kde4_add_unit_test( valuecodectest  TESTNAME libokteta-core-valuecodectest  ${valuecodectest_SRCS})
target_link_libraries( valuecodectest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### charcodectest ###############

set( charcodectest_SRCS
  charcodectest.cpp
)

kde4_add_unit_test( charcodectest  TESTNAME libokteta-core-charcodectest  ${charcodectest_SRCS})
target_link_libraries( charcodectest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### textcharcodectest ###############

set( textcharcodectest_SRCS
  textcharcodectest.cpp
)

kde4_add_unit_test( textcharcodectest  TESTNAME libokteta-core-textcharcodectest  ${textcharcodectest_SRCS} )
target_link_libraries( textcharcodectest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### textcharcodeccharcodeciftest ###############

set( textcharcodeccharcodeciftest_SRCS
  charcodeciftest.cpp
  textcharcodeccharcodeciftest.cpp
)

kde4_add_unit_test( textcharcodeccharcodeciftest TESTNAME libokteta-core-textcharcodeccharcodeciftest  ${textcharcodeccharcodeciftest_SRCS} )
target_link_libraries( textcharcodeccharcodeciftest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY}
)


########### ebcdic1047charcodeccharcodeciftest ###############

set( ebcdic1047charcodeccharcodeciftest_SRCS
  charcodeciftest.cpp ebcdic1047charcodeccharcodeciftest.cpp
)

kde4_add_unit_test( ebcdic1047charcodeccharcodeciftest  TESTNAME libokteta-core-ebcdic1047charcodeccharcodeciftest  ${ebcdic1047charcodeccharcodeciftest_SRCS} )
target_link_libraries( ebcdic1047charcodeccharcodeciftest
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


