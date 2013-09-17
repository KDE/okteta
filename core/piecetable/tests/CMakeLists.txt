set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/../.. )

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions(-DOKTETACORE_EXPORT=)

include_directories(
  ${SUTDIR}
)

add_library( testpiecetablechangelib STATIC
  abstractpiecetablechangeiftest.cpp
  testpiecetablechange.cpp
)
target_link_libraries( testpiecetablechangelib Qt5::Core testoktetacore )

########### piecetest ###############

set( piecetest_SRCS
  piecetest.cpp
)

kde4_add_unit_test( piecetest  TESTNAME libpiecetable-piecetest  ${piecetest_SRCS} )
target_link_libraries( piecetest Qt5::Test )


########### piecetabletest ###############

set( piecetabletest_SRCS
  piecetabletest.cpp
)

kde4_add_unit_test( piecetabletest  TESTNAME libpiecetable-piecetabletest  ${piecetabletest_SRCS} )
target_link_libraries( piecetabletest testoktetacore testpiecetablechangelib Qt5::Test KF5::KI18n )


########### testpiecetablechangetest ###############

set( testpiecetablechangetest_SRCS
  testpiecetablechangetest.cpp
)

kde4_add_unit_test( testpiecetablechangetest
  TESTNAME libpiecetable-testpiecetablechangetest
  ${testpiecetablechangetest_SRCS}
)
target_link_libraries( testpiecetablechangetest  oktetacoretestutil testpiecetablechangelib Qt5::Test KF5::KI18n )

########### testpiecetablechangeabstractpiecetablechangeiftest ###############

set( testpiecetablechangeabstractpiecetablechangeiftest_SRCS
  testpiecetablechangeabstractpiecetablechangeiftest.cpp
)

kde4_add_unit_test( testpiecetablechangeabstractpiecetablechangeiftest
  TESTNAME libpiecetable-testpiecetablechangeabstractpiecetablechangeiftest
  ${testpiecetablechangeabstractpiecetablechangeiftest_SRCS}
)
target_link_libraries( testpiecetablechangeabstractpiecetablechangeiftest
  oktetacoretestutil testoktetacore testpiecetablechangelib Qt5::Test )


########### insertpiecetablechangeabstractpiecetablechangeiftest ###############

set( insertpiecetablechangeabstractpiecetablechangeiftest_SRCS
  insertpiecetablechangeabstractpiecetablechangeiftest.cpp
)

kde4_add_unit_test( insertpiecetablechangeabstractpiecetablechangeiftest
  TESTNAME libpiecetable-insertpiecetablechangeabstractpiecetablechangeiftest
  ${insertpiecetablechangeabstractpiecetablechangeiftest_SRCS}
)
target_link_libraries( insertpiecetablechangeabstractpiecetablechangeiftest
  oktetacoretestutil testoktetacore testpiecetablechangelib Qt5::Test KF5::KI18n )


########### removepiecetablechangeabstractpiecetablechangeiftest ###############

set( removepiecetablechangeabstractpiecetablechangeiftest_SRCS
  removepiecetablechangeabstractpiecetablechangeiftest.cpp
)

kde4_add_unit_test( removepiecetablechangeabstractpiecetablechangeiftest
  TESTNAME libpiecetable-removepiecetablechangeabstractpiecetablechangeiftest
  ${removepiecetablechangeabstractpiecetablechangeiftest_SRCS}
)
target_link_libraries( removepiecetablechangeabstractpiecetablechangeiftest
  oktetacoretestutil testoktetacore testpiecetablechangelib Qt5::Test KF5::KI18n )


########### replacepiecetablechangeabstractpiecetablechangeiftest ###############

set( replacepiecetablechangeabstractpiecetablechangeiftest_SRCS
  replacepiecetablechangeabstractpiecetablechangeiftest.cpp
)

kde4_add_unit_test( replacepiecetablechangeabstractpiecetablechangeiftest
  TESTNAME libpiecetable-replacepiecetablechangeabstractpiecetablechangeiftest
  ${replacepiecetablechangeabstractpiecetablechangeiftest_SRCS}
)
target_link_libraries( replacepiecetablechangeabstractpiecetablechangeiftest
  oktetacoretestutil testoktetacore testpiecetablechangelib Qt5::Test KF5::KI18n )


########### swaprangespiecetablechangeabstractpiecetablechangeiftest ###############

set( swaprangespiecetablechangeabstractpiecetablechangeiftest_SRCS
  swaprangespiecetablechangeabstractpiecetablechangeiftest.cpp
)

kde4_add_unit_test( swaprangespiecetablechangeabstractpiecetablechangeiftest
  TESTNAME libpiecetable-swaprangespiecetablechangeabstractpiecetablechangeiftest
  ${swaprangespiecetablechangeabstractpiecetablechangeiftest_SRCS}
)
target_link_libraries( swaprangespiecetablechangeabstractpiecetablechangeiftest
  oktetacoretestutil testoktetacore testpiecetablechangelib Qt5::Test KF5::KI18n )


########### grouppiecetablechangeabstractpiecetablechangeiftest ###############

# grouped changes break some assumptions and got workarounds, so disable for now
if(IWILLPICKUPWORKONOKTETACORE)
set( grouppiecetablechangeabstractpiecetablechangeiftest_SRCS
  grouppiecetablechangeabstractpiecetablechangeiftest.cpp
)

kde4_add_unit_test( grouppiecetablechangeabstractpiecetablechangeiftest
  TESTNAME libpiecetable-grouppiecetablechangeabstractpiecetablechangeiftest
  ${grouppiecetablechangeabstractpiecetablechangeiftest_SRCS}
)
target_link_libraries( grouppiecetablechangeabstractpiecetablechangeiftest
  oktetacoretestutil testoktetacore testpiecetablechangelib Qt5::Test )
endif()

########### grouppiecetablechangetest ###############

set( grouppiecetablechangetest_SRCS
  grouppiecetablechangetest.cpp
)

kde4_add_unit_test( grouppiecetablechangetest
  TESTNAME libpiecetable-grouppiecetablechangetest
  ${grouppiecetablechangetest_SRCS}
)
target_link_libraries( grouppiecetablechangetest testoktetacore testpiecetablechangelib Qt5::Test KF5::KI18n)


########### piecetablechangehistorytest ###############

set( piecetablechangehistorytest_SRCS
  piecetablechangehistorytest.cpp
)

kde4_add_unit_test( piecetablechangehistorytest
  TESTNAME libpiecetable-piecetablechangehistorytest
  ${piecetablechangehistorytest_SRCS}
)
target_link_libraries( piecetablechangehistorytest testoktetacore testpiecetablechangelib Qt5::Test )


########### revertablepiecetabletest ###############

set( revertablepiecetabletest_SRCS
  revertablepiecetabletest.cpp
)

kde4_add_unit_test( revertablepiecetabletest
  TESTNAME libpiecetable-revertablepiecetabletest
  ${revertablepiecetabletest_SRCS}
)
target_link_libraries( revertablepiecetabletest testoktetacore Qt5::Test KF5::KI18n )
