set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( beiftest_SRCS beiftest.cpp )

kde4_add_executable(beiftest TEST  ${beiftest_SRCS} )

target_link_libraries( beiftest  ${KDE4_KPARTS_LIBS} ${KDE4Support_LIBRARIES} KF5::KI18n)

