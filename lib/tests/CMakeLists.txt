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

set(kbuffercoordtest_SRCS kbuffercoordtest.cpp )

kde4_automoc(${kbuffercoordtest_SRCS})

kde4_add_executable(kbuffercoordtest RUN_UNINSTALLED ${kbuffercoordtest_SRCS})

target_link_libraries(kbuffercoordtest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kbuffercoordtest ${EXECUTABLE_OUTPUT_PATH}/kbuffercoordtest)


########### next target ###############

set(kfixedsizebuffertest_SRCS kfixedsizebuffertest.cpp )

kde4_automoc(${kfixedsizebuffertest_SRCS})

kde4_add_executable(kfixedsizebuffertest RUN_UNINSTALLED ${kfixedsizebuffertest_SRCS})

target_link_libraries(kfixedsizebuffertest
  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kfixedsizebuffertest ${EXECUTABLE_OUTPUT_PATH}/kfixedsizebuffertest)


########### next target ###############

set(kfixedsizebufferkdatabufferiftest_SRCS
kdatabufferiftest.cpp kfixedsizebufferkdatabufferiftest.cpp )

kde4_automoc(${kfixedsizebufferkdatabufferiftest_SRCS})

kde4_add_executable(kfixedsizebufferkdatabufferiftest RUN_UNINSTALLED ${kfixedsizebufferkdatabufferiftest_SRCS})

target_link_libraries(kfixedsizebufferkdatabufferiftest
  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kfixedsizebufferkdatabufferiftest ${EXECUTABLE_OUTPUT_PATH}/kfixedsizebufferkdatabufferiftest)


########### next target ###############

set(kplainbuffertest_SRCS kplainbuffertest.cpp )

kde4_automoc(${kplainbuffertest_SRCS})

kde4_add_executable(kplainbuffertest RUN_UNINSTALLED ${kplainbuffertest_SRCS})

target_link_libraries(kplainbuffertest
  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kplainbuffertest ${EXECUTABLE_OUTPUT_PATH}/kplainbuffertest)


########### next target ###############

set(kplainbufferkdatabufferiftest_SRCS
kdatabufferiftest.cpp kplainbufferkdatabufferiftest.cpp )

kde4_automoc(${kplainbufferkdatabufferiftest_SRCS})

kde4_add_executable(kplainbufferkdatabufferiftest RUN_UNINSTALLED ${kplainbufferkdatabufferiftest_SRCS})

target_link_libraries(kplainbufferkdatabufferiftest
  testkhexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kplainbufferkdatabufferiftest ${EXECUTABLE_OUTPUT_PATH}/kplainbufferkdatabufferiftest)


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

