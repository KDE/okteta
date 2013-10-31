set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( beiftest_SRCS beiftest.cpp )

kde4_add_executable(beiftest TEST  ${beiftest_SRCS} )

target_link_libraries( beiftest KF5::KDE4Support Qt5::Widgets KF5::KI18n KF5::KService)

