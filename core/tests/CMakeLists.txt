set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

include_directories( ${SUTDIR} )


########### next target ###############

set(ksectiontest_SRCS ksectiontest.cpp
${SUTDIR}/ksection.cpp )

kde4_add_unit_test(ksectiontest TESTNAME okteta-ksectiontest  ${ksectiontest_SRCS})

target_link_libraries(ksectiontest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})


########### next target ###############

set( ksectionlisttest_SRCS ksectionlisttest.cpp )

kde4_add_unit_test(ksectionlisttest TESTNAME okteta-ksectionlisttest  ${ksectionlisttest_SRCS})

target_link_libraries(ksectionlisttest  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

########### next target ###############

set( kselectiontest_SRCS kselectiontest.cpp
${SUTDIR}/ksection.cpp )

kde4_add_unit_test(kselectiontest TESTNAME okteta-kselectiontest  ${kselectiontest_SRCS})

target_link_libraries(kselectiontest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

########### next target ###############

set(kfixedsizebytearraymodeltest_SRCS kfixedsizebytearraymodeltest.cpp )

kde4_add_unit_test(kfixedsizebytearraymodeltest TESTNAME okteta-kfixedsizebytearraymodeltest  ${kfixedsizebytearraymodeltest_SRCS})

target_link_libraries(kfixedsizebytearraymodeltest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

########### next target ###############

set(kfixedsizebytearraymodelkabstractbytearraymodeliftest_SRCS
kabstractbytearraymodeliftest.cpp kfixedsizebytearraymodelkabstractbytearraymodeliftest.cpp )

kde4_automoc(${kfixedsizebytearraymodelkabstractbytearraymodeliftest_SRCS})

kde4_add_unit_test(kfixedsizebytearraymodelkabstractbytearraymodeliftest TESTNAME okteta-kfixedsizebytearraymodelkabstractbytearraymodeliftest  ${kfixedsizebytearraymodelkabstractbytearraymodeliftest_SRCS})

target_link_libraries(kfixedsizebytearraymodelkabstractbytearraymodeliftest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

########### next target ###############

set(kbytearraymodeltest_SRCS kbytearraymodeltest.cpp )

kde4_add_unit_test(kbytearraymodeltest TESTNAME okteta-kbytearraymodeltest  ${kbytearraymodeltest_SRCS})

target_link_libraries(kbytearraymodeltest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})


########### next target ###############

set(kbytearraymodelkabstractbytearraymodeliftest_SRCS
kabstractbytearraymodeliftest.cpp kbytearraymodelkabstractbytearraymodeliftest.cpp )

kde4_add_unit_test(kbytearraymodelkabstractbytearraymodeliftest TESTNAME okteta-kbytearraymodelkabstractbytearraymodeliftest  ${kbytearraymodelkabstractbytearraymodeliftest_SRCS})

target_link_libraries(kbytearraymodelkabstractbytearraymodeliftest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

########### next target ###############

set(kcharcodectest_SRCS kcharcodectest.cpp )

kde4_add_unit_test(kcharcodectest TESTNAME okteta-kcharcodectest  ${kcharcodectest_SRCS})

target_link_libraries(kcharcodectest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

########### next target ###############

set(ktextcharcodectest_SRCS ktextcharcodectest.cpp )

kde4_add_unit_test(ktextcharcodectest TESTNAME okteta-ktextcharcodectest  ${ktextcharcodectest_SRCS})

target_link_libraries(ktextcharcodectest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

########### next target ###############

set(ktextcharcodeckcharcodeciftest_SRCS
kcharcodeciftest.cpp ktextcharcodeckcharcodeciftest.cpp )

kde4_automoc(${ktextcharcodeckcharcodeciftest_SRCS})

kde4_add_unit_test(ktextcharcodeckcharcodeciftest TESTNAME okteta-ktextcharcodeckcharcodeciftest  ${ktextcharcodeckcharcodeciftest_SRCS})

target_link_libraries(ktextcharcodeckcharcodeciftest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

########### next target ###############

set(kebcdic1047charcodeckcharcodeciftest_SRCS
kcharcodeciftest.cpp kebcdic1047charcodeckcharcodeciftest.cpp )

kde4_automoc(${kebcdic1047charcodeckcharcodeciftest_SRCS})

kde4_add_unit_test(kebcdic1047charcodeckcharcodeciftest TESTNAME okteta-kebcdic1047charcodeckcharcodeciftest  ${kebcdic1047charcodeckcharcodeciftest_SRCS})

target_link_libraries(kebcdic1047charcodeckcharcodeciftest
  testoktetacore ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})
