set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

include_directories(
  ${SUTDIR}
)



########### kselectiontest ###############

set( kselectiontest_SRCS
  kselectiontest.cpp
  ${SUTDIR}/../core/ksection.cpp
)

kde4_add_unit_test( kselectiontest  TESTNAME libokteta-gui-kselectiontest  ${kselectiontest_SRCS})
target_link_libraries( kselectiontest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### next target ###############

set(kcoordtest_SRCS kcoordtest.cpp )


kde4_add_unit_test(kcoordtest TESTNAME libokteta-gui-kcoordtest  ${kcoordtest_SRCS})

target_link_libraries(kcoordtest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})


########### next target ###############

set( kcoordrangetest_SRCS kcoordrangetest.cpp )

kde4_add_unit_test(kcoordrangetest TESTNAME libokteta-gui-kcoordrangetest  ${kcoordrangetest_SRCS})

target_link_libraries(kcoordrangetest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})


########### next target ###############

set( kcoordrangelisttest_SRCS
  ${SUTDIR}/kcoordrangelist.cpp
  kcoordrangelisttest.cpp
)

kde4_add_unit_test(kcoordrangelisttest TESTNAME libokteta-gui-kcoordrangelisttest  ${kcoordrangelisttest_SRCS})

target_link_libraries(kcoordrangelisttest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})


########### next target ###############

#set( mvctest_SRCS mvctest.cpp )

##kde4_add_unit_test(mvctest TESTNAME okteta-mvctest  ${mvctest_SRCS} )

# target_link_libraries( mvctest  oktetacore oktetagui ${KDE4_KDEUI_LIBS} )

########### install files ###############
