set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

include_directories(
  ${CMAKE_SOURCE_DIR}/khexedit/core
  ${SUTDIR}
)



########### next target ###############

set(kcoordtest_SRCS kcoordtest.cpp )


kde4_add_unit_test(kcoordtest TESTNAME okteta-kcoordtest  ${kcoordtest_SRCS})

target_link_libraries(kcoordtest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})


########### next target ###############

set( kcoordrangetest_SRCS kcoordrangetest.cpp )

kde4_add_unit_test(kcoordrangetest TESTNAME okteta-kcoordrangetest  ${kcoordrangetest_SRCS})

target_link_libraries(kcoordrangetest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})


########### next target ###############

set( kcoordrangelisttest_SRCS
  ${SUTDIR}/kcoordrangelist.cpp
  kcoordrangelisttest.cpp
)

kde4_add_unit_test(kcoordrangelisttest TESTNAME okteta-kcoordrangelisttest  ${kcoordrangelisttest_SRCS})

target_link_libraries(kcoordrangelisttest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})


########### next target ###############

#set( mvctest_SRCS mvctest.cpp )

##kde4_add_unit_test(mvctest TESTNAME okteta-mvctest  ${mvctest_SRCS} )

# target_link_libraries( mvctest  oktetacore oktetagui ${KDE4_KDEUI_LIBS} )

########### install files ###############
