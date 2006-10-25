if(KDE4_BUILD_TESTS)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


set( beiftest_SRCS beiftest.cpp )

kde4_automoc(${beiftest_SRCS})

kde4_add_executable( beiftest RUN_UNINSTALLED ${beiftest_SRCS} )

target_link_libraries( beiftest  ${KDE4_KPARTS_LIBS} )


endif(KDE4_BUILD_TESTS)
