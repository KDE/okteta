if(KDE4_BUILD_TESTS)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

include_directories(
  ${CMAKE_SOURCE_DIR}/khexedit/core
  ${SUTDIR}
)



########### next target ###############

set(kcoordtest_SRCS kcoordtest.cpp )

kde4_automoc(${kcoordtest_SRCS})

kde4_add_executable(kcoordtest RUN_UNINSTALLED ${kcoordtest_SRCS})

target_link_libraries(kcoordtest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kcoordtest ${EXECUTABLE_OUTPUT_PATH}/kcoordtest)


########### next target ###############

set( kcoordrangetest_SRCS kcoordrangetest.cpp )

kde4_automoc(${kcoordrangetest_SRCS})

kde4_add_executable(kcoordrangetest RUN_UNINSTALLED ${kcoordrangetest_SRCS})

target_link_libraries(kcoordrangetest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kcoordrangetest ${EXECUTABLE_OUTPUT_PATH}/kcoordrangetest)


########### next target ###############

set( kcoordrangelisttest_SRCS
  ${SUTDIR}/kcoordrangelist.cpp
  kcoordrangelisttest.cpp
)

kde4_automoc(${kcoordrangelisttest_SRCS})

kde4_add_executable(kcoordrangelisttest RUN_UNINSTALLED ${kcoordrangelisttest_SRCS})

target_link_libraries(kcoordrangelisttest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY})

add_test(khexedit-kcoordrangelisttest ${EXECUTABLE_OUTPUT_PATH}/kcoordrangelisttest)


########### next target ###############

#set( mvctest_SRCS mvctest.cpp )

#kde4_automoc( ${mvctest_SRCS} )

#kde4_add_executable( mvctest RUN_UNINSTALLED ${mvctest_SRCS} )

# target_link_libraries( mvctest  khexeditcore khexeditgui ${KDE4_KDEUI_LIBS} )

########### install files ###############

endif(KDE4_BUILD_TESTS)

