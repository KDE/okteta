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
target_link_libraries( arraychangemetricstest Qt5::Test )


########### addressrangetest ###############

set( addressrangetest_SRCS
  addressrangetest.cpp
)

kde4_add_unit_test( addressrangetest  TESTNAME libokteta-core-addressrangetest  ${addressrangetest_SRCS} )
target_link_libraries( addressrangetest testoktetacore Qt5::Test )


########### addressrangelisttest ###############

set( addressrangelisttest_SRCS
  addressrangelisttest.cpp
)

kde4_add_unit_test( addressrangelisttest  TESTNAME libokteta-core-addressrangelisttest  ${addressrangelisttest_SRCS} )
target_link_libraries( addressrangelisttest  testoktetacore Qt5::Test )


########### fixedsizebytearraymodeltest ###############

set( fixedsizebytearraymodeltest_SRCS
  fixedsizebytearraymodeltest.cpp
)

kde4_add_unit_test( fixedsizebytearraymodeltest  TESTNAME libokteta-core-fixedsizebytearraymodeltest  ${fixedsizebytearraymodeltest_SRCS})
target_link_libraries( fixedsizebytearraymodeltest
  testoktetacore Qt5::Test KF5::KI18n
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
  oktetacoretestutil testoktetacore Qt5::Test KF5::KI18n
)


########### bytearraymodeltest ###############

set( bytearraymodeltest_SRCS
  bytearraymodeltest.cpp
)

kde4_add_unit_test( bytearraymodeltest  TESTNAME libokteta-core-bytearraymodeltest  ${bytearraymodeltest_SRCS} )
target_link_libraries( bytearraymodeltest
  ${oktetacore_LIB} Qt5::Test
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
  oktetacoretestutil testoktetacore Qt5::Test KF5::KI18n
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
  oktetacoretestutil testoktetacore Qt5::Test KF5::KI18n
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
  oktetacoretestutil ${oktetacore_LIB} Qt5::Test
)


########### valuecodectest ###############

set( valuecodectest_SRCS
  valuecodectest.cpp
)

kde4_add_unit_test( valuecodectest  TESTNAME libokteta-core-valuecodectest  ${valuecodectest_SRCS})
target_link_libraries( valuecodectest
  ${oktetacore_LIB} Qt5::Test
)


########### charcodectest ###############

set( charcodectest_SRCS
  charcodectest.cpp
)

kde4_add_unit_test( charcodectest  TESTNAME libokteta-core-charcodectest  ${charcodectest_SRCS})
target_link_libraries( charcodectest
  ${oktetacore_LIB} Qt5::Test KF5::KI18n
)


########### textcharcodectest ###############

set( textcharcodectest_SRCS
  textcharcodectest.cpp
)

kde4_add_unit_test( textcharcodectest  TESTNAME libokteta-core-textcharcodectest  ${textcharcodectest_SRCS} )
target_link_libraries( textcharcodectest
  testoktetacore Qt5::Test KF5::KI18n KF5::KCodecs KF5::KDE4Attic
)


########### textcharcodeccharcodeciftest ###############

set( textcharcodeccharcodeciftest_SRCS
  charcodeciftest.cpp
  textcharcodeccharcodeciftest.cpp
)

kde4_add_unit_test( textcharcodeccharcodeciftest TESTNAME libokteta-core-textcharcodeccharcodeciftest  ${textcharcodeccharcodeciftest_SRCS} )
target_link_libraries( textcharcodeccharcodeciftest
  testoktetacore Qt5::Test KF5::KI18n KF5::KCodecs KF5::KDE4Attic
)


########### ebcdic1047charcodeccharcodeciftest ###############

set( ebcdic1047charcodeccharcodeciftest_SRCS
  charcodeciftest.cpp ebcdic1047charcodeccharcodeciftest.cpp
)

kde4_add_unit_test( ebcdic1047charcodeccharcodeciftest  TESTNAME libokteta-core-ebcdic1047charcodeccharcodeciftest  ${ebcdic1047charcodeccharcodeciftest_SRCS} )
target_link_libraries( ebcdic1047charcodeccharcodeciftest
  testoktetacore Qt5::Test
)


########### bookmarktest ###############

set( bookmarktest_SRCS
  bookmarktest.cpp
)

kde4_add_unit_test( bookmarktest  TESTNAME libokteta-core-bookmarktest  ${bookmarktest_SRCS} )
target_link_libraries( bookmarktest Qt5::Test )


########### bookmarklisttest ###############

set( bookmarklisttest_SRCS
  bookmarklisttest.cpp
)

kde4_add_unit_test( bookmarklisttest  TESTNAME libokteta-core-bookmarklisttest  ${bookmarklisttest_SRCS} )
target_link_libraries( bookmarklisttest testoktetacore Qt5::Test KF5::KI18n )


