
include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} )


set( beiftest_SRCS beiftest.cpp )

kde4_add_test_executable(beiftest  ${beiftest_SRCS} )

target_link_libraries( beiftest  ${KDE4_KPARTS_LIBS} )

