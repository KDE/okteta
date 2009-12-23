set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions( -DOKTETACORE_EXPORT= )
add_definitions( -DOKTETAGUI_EXPORT= )

include_directories(
  ${SUTDIR}
)


########### selectiontest ###############

set( selectiontest_SRCS
  selectiontest.cpp
)

kde4_add_unit_test( selectiontest  TESTNAME libokteta-gui-selectiontest  ${selectiontest_SRCS} )
target_link_libraries( selectiontest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )

set_tests_properties(libokteta-gui-selectiontest PROPERTIES LABELS OktetaLabel)

########### coordtest ###############

set( coordtest_SRCS
  coordtest.cpp
)

kde4_add_unit_test( coordtest  TESTNAME libokteta-gui-coordtest  ${coordtest_SRCS} )
target_link_libraries( coordtest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### coordrangetest ###############

set( coordrangetest_SRCS
  coordrangetest.cpp
)

kde4_add_unit_test( coordrangetest  TESTNAME libokteta-gui-coordrangetest  ${coordrangetest_SRCS} )
target_link_libraries( coordrangetest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### coordrangelisttest ###############

set( coordrangelisttest_SRCS
  ${SUTDIR}/coordrangelist.cpp
  coordrangelisttest.cpp
)

kde4_add_unit_test( coordrangelisttest  TESTNAME libokteta-gui-coordrangelisttest  ${coordrangelisttest_SRCS} )
target_link_libraries( coordrangelisttest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )


########### bytearraytablelayouttest ###############

set( bytearraytablelayouttest_SRCS
  ${SUTDIR}/bytearraytablelayout.cpp
  bytearraytablelayouttest.cpp
)

kde4_add_unit_test( bytearraytablelayouttest
  TESTNAME libokteta-gui-bytearraytablelayouttest
  ${bytearraytablelayouttest_SRCS}
)
target_link_libraries( bytearraytablelayouttest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )
# FIXME:
# this is a rather stupid workaround - you should link to liboktetagui instead
set_target_properties( bytearraytablelayouttest PROPERTIES COMPILE_FLAGS "-DMAKE_OKTETAGUI_LIB" )


########### bytearraytablecursortest ###############

set( bytearraytablecursortest_SRCS
  ${SUTDIR}/bytearraytablecursor.cpp
  ${SUTDIR}/bytearraytablelayout.cpp
  bytearraytablecursortest.cpp
)

kde4_add_unit_test( bytearraytablecursortest
  TESTNAME libokteta-gui-bytearraytablecursortest
  ${bytearraytablecursortest_SRCS}
)
target_link_libraries( bytearraytablecursortest  ${KDE4_KDECORE_LIBS} ${QT_QTTEST_LIBRARY} )
# FIXME:
# this is a rather stupid workaround - you should link to liboktetagui instead
set_target_properties( bytearraytablecursortest PROPERTIES COMPILE_FLAGS "-DMAKE_OKTETAGUI_LIB" )


########### mvctest ###############

#set( mvctest_SRCS mvctest.cpp )

##kde4_add_unit_test(mvctest TESTNAME okteta-mvctest  ${mvctest_SRCS} )

# target_link_libraries( mvctest  oktetacore oktetagui ${KDE4_KDEUI_LIBS} )

########### install files ###############
