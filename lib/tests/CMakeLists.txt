if(KDE4_BUILD_TESTS)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

include_directories( ${SUTDIR} )


########### next target ###############

set(ksectiontest_SRCS ksectiontest.cpp
${SUTDIR}/ksection.cpp )

kde4_automoc(${ksectiontest_SRCS})

kde4_add_executable(ksectiontest RUN_UNINSTALLED ${ksectiontest_SRCS})

target_link_libraries(ksectiontest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-ksectiontest ${EXECUTABLE_OUTPUT_PATH}/ksectiontest)


########### next target ###############

set( ksectionlisttest_SRCS ksectionlisttest.cpp )

kde4_automoc(${ksectionlisttest_SRCS})

kde4_add_executable(ksectionlisttest RUN_UNINSTALLED ${ksectionlisttest_SRCS})

target_link_libraries(ksectionlisttest  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-ksectionlisttest ${EXECUTABLE_OUTPUT_PATH}/ksectionlisttest)


########### next target ###############

set( kselectiontest_SRCS kselectiontest.cpp
${SUTDIR}/ksection.cpp )

kde4_automoc(${kselectiontest_SRCS})

kde4_add_executable(kselectiontest RUN_UNINSTALLED ${kselectiontest_SRCS})

target_link_libraries(kselectiontest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kselectiontest ${EXECUTABLE_OUTPUT_PATH}/kselectiontest)


########### next target ###############

set(kbuffercoordtest_SRCS kbuffercoordtest.cpp )

kde4_automoc(${kbuffercoordtest_SRCS})

kde4_add_executable(kbuffercoordtest RUN_UNINSTALLED ${kbuffercoordtest_SRCS})

target_link_libraries(kbuffercoordtest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kbuffercoordtest ${EXECUTABLE_OUTPUT_PATH}/kbuffercoordtest)


########### next target ###############

set( kcoordrangetest_SRCS kcoordrangetest.cpp )

kde4_automoc(${kcoordrangetest_SRCS})

kde4_add_executable(kcoordrangetest RUN_UNINSTALLED ${kcoordrangetest_SRCS})

target_link_libraries(kcoordrangetest  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kcoordrangetest ${EXECUTABLE_OUTPUT_PATH}/kcoordrangetest)


########### next target ###############

set( kcoordrangelisttest_SRCS kcoordrangelisttest.cpp )

kde4_automoc(${kcoordrangelisttest_SRCS})

kde4_add_executable(kcoordrangelisttest RUN_UNINSTALLED ${kcoordrangelisttest_SRCS})

target_link_libraries(kcoordrangelisttest  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kcoordrangelisttest ${EXECUTABLE_OUTPUT_PATH}/kcoordrangelisttest)


########### next target ###############

set(kfixedsizebytearraymodeltest_SRCS kfixedsizebytearraymodeltest.cpp )

kde4_automoc(${kfixedsizebytearraymodeltest_SRCS})

kde4_add_executable(kfixedsizebytearraymodeltest RUN_UNINSTALLED ${kfixedsizebytearraymodeltest_SRCS})

target_link_libraries(kfixedsizebytearraymodeltest
  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kfixedsizebytearraymodeltest ${EXECUTABLE_OUTPUT_PATH}/kfixedsizebytearraymodeltest)


########### next target ###############

set(kfixedsizebytearraymodelkabstractbytearraymodeliftest_SRCS
kabstractbytearraymodeliftest.cpp kfixedsizebytearraymodelkabstractbytearraymodeliftest.cpp )

kde4_automoc(${kfixedsizebytearraymodelkabstractbytearraymodeliftest_SRCS})

kde4_add_executable(kfixedsizebytearraymodelkabstractbytearraymodeliftest RUN_UNINSTALLED ${kfixedsizebytearraymodelkabstractbytearraymodeliftest_SRCS})

target_link_libraries(kfixedsizebytearraymodelkabstractbytearraymodeliftest
  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kfixedsizebytearraymodelkabstractbytearraymodeliftest ${EXECUTABLE_OUTPUT_PATH}/kfixedsizebytearraymodelkabstractbytearraymodeliftest)


########### next target ###############

set(kbytearraymodeltest_SRCS kbytearraymodeltest.cpp )

kde4_automoc(${kbytearraymodeltest_SRCS})

kde4_add_executable(kbytearraymodeltest RUN_UNINSTALLED ${kbytearraymodeltest_SRCS})

target_link_libraries(kbytearraymodeltest
  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kbytearraymodeltest ${EXECUTABLE_OUTPUT_PATH}/kbytearraymodeltest)


########### next target ###############

set(kbytearraymodelkabstractbytearraymodeliftest_SRCS
kabstractbytearraymodeliftest.cpp kbytearraymodelkabstractbytearraymodeliftest.cpp )

kde4_automoc(${kbytearraymodelkabstractbytearraymodeliftest_SRCS})

kde4_add_executable(kbytearraymodelkabstractbytearraymodeliftest RUN_UNINSTALLED ${kbytearraymodelkabstractbytearraymodeliftest_SRCS})

target_link_libraries(kbytearraymodelkabstractbytearraymodeliftest
  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kbytearraymodelkabstractbytearraymodeliftest ${EXECUTABLE_OUTPUT_PATH}/kbytearraymodelkabstractbytearraymodeliftest)


########### next target ###############

set(kcharcodectest_SRCS kcharcodectest.cpp )

kde4_automoc(${kcharcodectest_SRCS})

kde4_add_executable(kcharcodectest RUN_UNINSTALLED ${kcharcodectest_SRCS})

target_link_libraries(kcharcodectest
  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kcharcodectest ${EXECUTABLE_OUTPUT_PATH}/kcharcodectest)


########### next target ###############

set(ktextcharcodectest_SRCS ktextcharcodectest.cpp )

kde4_automoc(${ktextcharcodectest_SRCS})

kde4_add_executable(ktextcharcodectest RUN_UNINSTALLED ${ktextcharcodectest_SRCS})

target_link_libraries(ktextcharcodectest
  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-ktextcharcodectest ${EXECUTABLE_OUTPUT_PATH}/ktextcharcodectest)


########### next target ###############

set(ktextcharcodeckcharcodeciftest_SRCS
kcharcodeciftest.cpp ktextcharcodeckcharcodeciftest.cpp )

kde4_automoc(${ktextcharcodeckcharcodeciftest_SRCS})

kde4_add_executable(ktextcharcodeckcharcodeciftest RUN_UNINSTALLED ${ktextcharcodeckcharcodeciftest_SRCS})

target_link_libraries(ktextcharcodeckcharcodeciftest
  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-ktextcharcodeckcharcodeciftest ${EXECUTABLE_OUTPUT_PATH}/ktextcharcodeckcharcodeciftest)


########### next target ###############

set(kebcdic1047charcodeckcharcodeciftest_SRCS
kcharcodeciftest.cpp kebcdic1047charcodeckcharcodeciftest.cpp )

kde4_automoc(${kebcdic1047charcodeckcharcodeciftest_SRCS})

kde4_add_executable(kebcdic1047charcodeckcharcodeciftest RUN_UNINSTALLED ${kebcdic1047charcodeckcharcodeciftest_SRCS})

target_link_libraries(kebcdic1047charcodeckcharcodeciftest
  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kebcdic1047charcodeckcharcodeciftest ${EXECUTABLE_OUTPUT_PATH}/kebcdic1047charcodeckcharcodeciftest)


########### install files ###############

endif(KDE4_BUILD_TESTS)

