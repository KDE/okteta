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


########### install files ###############

endif(KDE4_BUILD_TESTS)

kde4_footer()
