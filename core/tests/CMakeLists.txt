set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

include_directories( ${SUTDIR} )


# helper
set( oktetacoretestutil_LIB_SRCS
  util/fill.cpp
)

kde4_add_library( oktetacoretestutil  STATIC ${oktetacoretestutil_LIB_SRCS} )


########### ksectiontest ###############

set( ksectiontest_SRCS
  ksectiontest.cpp
  ${SUTDIR}/ksection.cpp
)

kde4_add_unit_test( ksectiontest  TESTNAME libokteta-core-ksectiontest  ${ksectiontest_SRCS} )
target_link_libraries( ksectiontest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### ksectionlisttest ###############

set( ksectionlisttest_SRCS
  ksectionlisttest.cpp
)

kde4_add_unit_test( ksectionlisttest  TESTNAME libokteta-core-ksectionlisttest  ${ksectionlisttest_SRCS} )
target_link_libraries( ksectionlisttest  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### kselectiontest ###############

set( kselectiontest_SRCS
  kselectiontest.cpp
  ${SUTDIR}/ksection.cpp
)

kde4_add_unit_test( kselectiontest  TESTNAME libokteta-core-kselectiontest  ${kselectiontest_SRCS})
target_link_libraries( kselectiontest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### kfixedsizebytearraymodeltest ###############

set( kfixedsizebytearraymodeltest_SRCS
  kfixedsizebytearraymodeltest.cpp
)

kde4_add_unit_test( kfixedsizebytearraymodeltest  TESTNAME libokteta-core-kfixedsizebytearraymodeltest  ${kfixedsizebytearraymodeltest_SRCS})
target_link_libraries( kfixedsizebytearraymodeltest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### kfixedsizebytearraymodelkabstractbytearraymodeliftest ###############

set( kfixedsizebytearraymodelkabstractbytearraymodeliftest_SRCS
  kabstractbytearraymodeliftest.cpp
  kfixedsizebytearraymodelkabstractbytearraymodeliftest.cpp
)

kde4_add_unit_test( kfixedsizebytearraymodelkabstractbytearraymodeliftest
  TESTNAME libokteta-core-kfixedsizebytearraymodelkabstractbytearraymodeliftest
  ${kfixedsizebytearraymodelkabstractbytearraymodeliftest_SRCS}
)
target_link_libraries( kfixedsizebytearraymodelkabstractbytearraymodeliftest
  oktetacoretestutil testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### kbytearraymodeltest ###############

set( kbytearraymodeltest_SRCS
  kbytearraymodeltest.cpp
)

kde4_add_unit_test( kbytearraymodeltest  TESTNAME libokteta-core-kbytearraymodeltest  ${kbytearraymodeltest_SRCS} )
target_link_libraries( kbytearraymodeltest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### kbytearraymodelkabstractbytearraymodeliftest ###############

set( kbytearraymodelkabstractbytearraymodeliftest_SRCS
  kabstractbytearraymodeliftest.cpp
  kbytearraymodelkabstractbytearraymodeliftest.cpp
)

kde4_add_unit_test( kbytearraymodelkabstractbytearraymodeliftest
  TESTNAME libokteta-core-kbytearraymodelkabstractbytearraymodeliftest
  ${kbytearraymodelkabstractbytearraymodeliftest_SRCS}
)
target_link_libraries( kbytearraymodelkabstractbytearraymodeliftest
  oktetacoretestutil testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### kcharcodectest ###############

set( kcharcodectest_SRCS
  kcharcodectest.cpp
)

kde4_add_unit_test( kcharcodectest  TESTNAME libokteta-core-kcharcodectest  ${kcharcodectest_SRCS})
target_link_libraries( kcharcodectest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### ktextcharcodectest ###############

set( ktextcharcodectest_SRCS
  ktextcharcodectest.cpp
)

kde4_add_unit_test( ktextcharcodectest  TESTNAME libokteta-core-ktextcharcodectest  ${ktextcharcodectest_SRCS} )
target_link_libraries( ktextcharcodectest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### ktextcharcodeckcharcodeciftest ###############

set( ktextcharcodeckcharcodeciftest_SRCS
  kcharcodeciftest.cpp
  ktextcharcodeckcharcodeciftest.cpp
)

kde4_add_unit_test( ktextcharcodeckcharcodeciftest TESTNAME libokteta-core-ktextcharcodeckcharcodeciftest  ${ktextcharcodeckcharcodeciftest_SRCS} )
target_link_libraries( ktextcharcodeckcharcodeciftest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})


########### kebcdic1047charcodeckcharcodeciftest ###############

set( kebcdic1047charcodeckcharcodeciftest_SRCS
  kcharcodeciftest.cpp kebcdic1047charcodeckcharcodeciftest.cpp
)

kde4_add_unit_test( kebcdic1047charcodeckcharcodeciftest  TESTNAME libokteta-core-kebcdic1047charcodeckcharcodeciftest  ${kebcdic1047charcodeckcharcodeciftest_SRCS} )
target_link_libraries( kebcdic1047charcodeckcharcodeciftest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )
