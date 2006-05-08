kde4_header()

if(KDE4_BUILD_TESTS)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. )


########### next target ###############

set(ksectiontest_SRCS ksectiontest.cpp )

kde4_automoc(${ksectiontest_SRCS})

kde4_add_executable(ksectiontest RUN_UNINSTALLED ${ksectiontest_SRCS})

target_link_libraries(ksectiontest  khexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-ksectiontest ${EXECUTABLE_OUTPUT_PATH}/ksectiontest)


########### next target ###############

set(kbuffercoordtest_SRCS kbuffercoordtest.cpp )

kde4_automoc(${kbuffercoordtest_SRCS})

kde4_add_executable(kbuffercoordtest RUN_UNINSTALLED ${kbuffercoordtest_SRCS})

target_link_libraries(kbuffercoordtest  khexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kbuffercoordtest ${EXECUTABLE_OUTPUT_PATH}/kbuffercoordtest)


########### next target ###############

set(kfixedsizebuffertest_SRCS kfixedsizebuffertest.cpp )

kde4_automoc(${kfixedsizebuffertest_SRCS})

kde4_add_executable(kfixedsizebuffertest RUN_UNINSTALLED ${kfixedsizebuffertest_SRCS})

target_link_libraries(kfixedsizebuffertest  khexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kfixedsizebuffertest ${EXECUTABLE_OUTPUT_PATH}/kfixedsizebuffertest)


########### next target ###############

set(kfixedsizebufferkdatabufferiftest_SRCS
kdatabufferiftest.cpp kfixedsizebufferkdatabufferiftest.cpp )

kde4_automoc(${kfixedsizebufferkdatabufferiftest_SRCS})

kde4_add_executable(kfixedsizebufferkdatabufferiftest RUN_UNINSTALLED ${kfixedsizebufferkdatabufferiftest_SRCS})

target_link_libraries(kfixedsizebufferkdatabufferiftest  khexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kfixedsizebufferkdatabufferiftest ${EXECUTABLE_OUTPUT_PATH}/kfixedsizebufferkdatabufferiftest)


########### next target ###############

set(kplainbuffertest_SRCS kplainbuffertest.cpp )

kde4_automoc(${kplainbuffertest_SRCS})

kde4_add_executable(kplainbuffertest RUN_UNINSTALLED ${kplainbuffertest_SRCS})

target_link_libraries(kplainbuffertest  khexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kplainbuffertest ${EXECUTABLE_OUTPUT_PATH}/kplainbuffertest)


########### next target ###############

set(kplainbufferkdatabufferiftest_SRCS
kdatabufferiftest.cpp kplainbufferkdatabufferiftest.cpp )

kde4_automoc(${kplainbufferkdatabufferiftest_SRCS})

kde4_add_executable(kplainbufferkdatabufferiftest RUN_UNINSTALLED ${kplainbufferkdatabufferiftest_SRCS})

target_link_libraries(kplainbufferkdatabufferiftest  khexeditcommon ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kplainbufferkdatabufferiftest ${EXECUTABLE_OUTPUT_PATH}/kplainbufferkdatabufferiftest)


########### install files ###############

endif(KDE4_BUILD_TESTS)

kde4_footer()
