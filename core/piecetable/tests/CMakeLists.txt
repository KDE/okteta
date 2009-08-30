set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/../.. )

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions(-DOKTETACORE_EXPORT=)

include_directories(
  ${SUTDIR}
)

########### piecetest ###############

set( piecetest_SRCS
  piecetest.cpp
)

kde4_add_unit_test( piecetest  TESTNAME libpiecetable-piecetest  ${piecetest_SRCS} )
target_link_libraries( piecetest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### piecetabletest ###############

set( piecetabletest_SRCS
  piecetabletest.cpp
  ${SUTDIR}/piecetable/piecetable.cpp
)

kde4_add_unit_test( piecetabletest  TESTNAME libpiecetable-piecetabletest  ${piecetabletest_SRCS} )
target_link_libraries( piecetabletest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### testpiecetablechangetest ###############

set( testpiecetablechangetest_SRCS
  testpiecetablechangetest.cpp
  testpiecetablechange.cpp
  ${SUTDIR}/piecetable/piecetable.cpp
  ${SUTDIR}/piecetable/abstractpiecetablechange.cpp
)

kde4_add_unit_test( testpiecetablechangetest
  TESTNAME libpiecetable-testpiecetablechangetest
  ${testpiecetablechangetest_SRCS}
)
target_link_libraries( testpiecetablechangetest  oktetacoretestutil ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### abstractpiecetablechangeiftestsources ###############

set( abstractpiecetablechangeiftest_SRCS
  abstractpiecetablechangeiftest.cpp
  testpiecetablechange.cpp
  ${SUTDIR}/piecetable/piecetable.cpp
  ${SUTDIR}/piecetable/abstractpiecetablechange.cpp
)

########### testpiecetablechangeabstractpiecetablechangeiftest ###############

set( testpiecetablechangeabstractpiecetablechangeiftest_SRCS
  testpiecetablechangeabstractpiecetablechangeiftest.cpp
  ${abstractpiecetablechangeiftest_SRCS}
)

kde4_add_unit_test( testpiecetablechangeabstractpiecetablechangeiftest
  TESTNAME libpiecetable-testpiecetablechangeabstractpiecetablechangeiftest
  ${testpiecetablechangeabstractpiecetablechangeiftest_SRCS}
)
target_link_libraries( testpiecetablechangeabstractpiecetablechangeiftest
  oktetacoretestutil ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### insertpiecetablechangeabstractpiecetablechangeiftest ###############

set( insertpiecetablechangeabstractpiecetablechangeiftest_SRCS
  insertpiecetablechangeabstractpiecetablechangeiftest.cpp
  ${abstractpiecetablechangeiftest_SRCS}
  ${SUTDIR}/piecetable/insertpiecetablechange.cpp
)

kde4_add_unit_test( insertpiecetablechangeabstractpiecetablechangeiftest
  TESTNAME libpiecetable-insertpiecetablechangeabstractpiecetablechangeiftest
  ${insertpiecetablechangeabstractpiecetablechangeiftest_SRCS}
)
target_link_libraries( insertpiecetablechangeabstractpiecetablechangeiftest
  oktetacoretestutil ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### removepiecetablechangeabstractpiecetablechangeiftest ###############

set( removepiecetablechangeabstractpiecetablechangeiftest_SRCS
  removepiecetablechangeabstractpiecetablechangeiftest.cpp
  ${abstractpiecetablechangeiftest_SRCS}
  ${SUTDIR}/piecetable/removepiecetablechange.cpp
)

kde4_add_unit_test( removepiecetablechangeabstractpiecetablechangeiftest
  TESTNAME libpiecetable-removepiecetablechangeabstractpiecetablechangeiftest
  ${removepiecetablechangeabstractpiecetablechangeiftest_SRCS}
)
target_link_libraries( removepiecetablechangeabstractpiecetablechangeiftest
  oktetacoretestutil ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### replacepiecetablechangeabstractpiecetablechangeiftest ###############

set( replacepiecetablechangeabstractpiecetablechangeiftest_SRCS
  replacepiecetablechangeabstractpiecetablechangeiftest.cpp
  ${abstractpiecetablechangeiftest_SRCS}
  ${SUTDIR}/piecetable/replacepiecetablechange.cpp
)

kde4_add_unit_test( replacepiecetablechangeabstractpiecetablechangeiftest
  TESTNAME libpiecetable-replacepiecetablechangeabstractpiecetablechangeiftest
  ${replacepiecetablechangeabstractpiecetablechangeiftest_SRCS}
)
target_link_libraries( replacepiecetablechangeabstractpiecetablechangeiftest
  oktetacoretestutil ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### swaprangespiecetablechangeabstractpiecetablechangeiftest ###############

set( swaprangespiecetablechangeabstractpiecetablechangeiftest_SRCS
  swaprangespiecetablechangeabstractpiecetablechangeiftest.cpp
  ${abstractpiecetablechangeiftest_SRCS}
  ${SUTDIR}/piecetable/swaprangespiecetablechange.cpp
)

kde4_add_unit_test( swaprangespiecetablechangeabstractpiecetablechangeiftest
  TESTNAME libpiecetable-swaprangespiecetablechangeabstractpiecetablechangeiftest
  ${swaprangespiecetablechangeabstractpiecetablechangeiftest_SRCS}
)
target_link_libraries( swaprangespiecetablechangeabstractpiecetablechangeiftest
  oktetacoretestutil ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### grouppiecetablechangeabstractpiecetablechangeiftest ###############

set( grouppiecetablechangeabstractpiecetablechangeiftest_SRCS
  grouppiecetablechangeabstractpiecetablechangeiftest.cpp
  ${abstractpiecetablechangeiftest_SRCS}
  ${SUTDIR}/addressrangelist.cpp
  ${SUTDIR}/piecetable/replacepiecetablechange.cpp
  ${SUTDIR}/piecetable/insertpiecetablechange.cpp
  ${SUTDIR}/piecetable/removepiecetablechange.cpp
  ${SUTDIR}/piecetable/grouppiecetablechange.cpp
)

kde4_add_unit_test( grouppiecetablechangeabstractpiecetablechangeiftest
  TESTNAME libpiecetable-grouppiecetablechangeabstractpiecetablechangeiftest
  ${grouppiecetablechangeabstractpiecetablechangeiftest_SRCS}
)
target_link_libraries( grouppiecetablechangeabstractpiecetablechangeiftest
  oktetacoretestutil ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### grouppiecetablechangetest ###############

set( grouppiecetablechangetest_SRCS
  grouppiecetablechangetest.cpp
  testpiecetablechange.cpp
  ${SUTDIR}/addressrangelist.cpp
  ${SUTDIR}/piecetable/piecetable.cpp
  ${SUTDIR}/piecetable/abstractpiecetablechange.cpp
  ${SUTDIR}/piecetable/insertpiecetablechange.cpp
  ${SUTDIR}/piecetable/removepiecetablechange.cpp
  ${SUTDIR}/piecetable/replacepiecetablechange.cpp
  ${SUTDIR}/piecetable/swaprangespiecetablechange.cpp
  ${SUTDIR}/piecetable/grouppiecetablechange.cpp
)

kde4_add_unit_test( grouppiecetablechangetest
  TESTNAME libpiecetable-grouppiecetablechangetest
  ${grouppiecetablechangetest_SRCS}
)
target_link_libraries( grouppiecetablechangetest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### piecetablechangehistorytest ###############

set( piecetablechangehistorytest_SRCS
  piecetablechangehistorytest.cpp
  testpiecetablechange.cpp
  ${SUTDIR}/addressrangelist.cpp
  ${SUTDIR}/piecetable/piecetable.cpp
  ${SUTDIR}/piecetable/abstractpiecetablechange.cpp
  ${SUTDIR}/piecetable/insertpiecetablechange.cpp
  ${SUTDIR}/piecetable/removepiecetablechange.cpp
  ${SUTDIR}/piecetable/replacepiecetablechange.cpp
  ${SUTDIR}/piecetable/swaprangespiecetablechange.cpp
  ${SUTDIR}/piecetable/piecetablechangehistory.cpp
  ${SUTDIR}/piecetable/grouppiecetablechange.cpp
)

kde4_add_unit_test( piecetablechangehistorytest
  TESTNAME libpiecetable-piecetablechangehistorytest
  ${piecetablechangehistorytest_SRCS}
)
target_link_libraries( piecetablechangehistorytest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### revertablepiecetabletest ###############

set( revertablepiecetabletest_SRCS
  revertablepiecetabletest.cpp
  ${SUTDIR}/addressrangelist.cpp
  ${SUTDIR}/piecetable/piecetable.cpp
  ${SUTDIR}/piecetable/abstractpiecetablechange.cpp
  ${SUTDIR}/piecetable/insertpiecetablechange.cpp
  ${SUTDIR}/piecetable/removepiecetablechange.cpp
  ${SUTDIR}/piecetable/replacepiecetablechange.cpp
  ${SUTDIR}/piecetable/swaprangespiecetablechange.cpp
  ${SUTDIR}/piecetable/piecetablechangehistory.cpp
  ${SUTDIR}/piecetable/revertablepiecetable.cpp
  ${SUTDIR}/piecetable/grouppiecetablechange.cpp
)

kde4_add_unit_test( revertablepiecetabletest
  TESTNAME libpiecetable-revertablepiecetabletest
  ${revertablepiecetabletest_SRCS}
)
target_link_libraries( revertablepiecetabletest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )
