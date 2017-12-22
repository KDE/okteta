set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/../.. )

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions(-DOKTETACORE_EXPORT=)

include_directories(
  ${SUTDIR}
)

add_library( testpiecetablechangelib STATIC
  abstractpiecetablechangeiftest.cpp
  testpiecetablechange.cpp
)
target_link_libraries( testpiecetablechangelib  testoktetacore Qt5::Test Qt5::Core )


########### piecetest ###############

ecm_add_test( piecetest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES Qt5::Test
)
target_include_directories(piecetest
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../..
)

########### piecetabletest ###############

ecm_add_test( piecetabletest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES testoktetacore testpiecetablechangelib Qt5::Test KF5::I18n
)

########### testpiecetablechangetest ###############

ecm_add_test( testpiecetablechangetest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES oktetacoretestutil testpiecetablechangelib Qt5::Test KF5::I18n
)

########### testpiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( testpiecetablechangeabstractpiecetablechangeiftest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES oktetacoretestutil testoktetacore testpiecetablechangelib Qt5::Test
)

########### insertpiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( insertpiecetablechangeabstractpiecetablechangeiftest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES oktetacoretestutil testoktetacore testpiecetablechangelib Qt5::Test KF5::I18n
)

########### removepiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( removepiecetablechangeabstractpiecetablechangeiftest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES oktetacoretestutil testoktetacore testpiecetablechangelib Qt5::Test KF5::I18n
)

########### replacepiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( replacepiecetablechangeabstractpiecetablechangeiftest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES oktetacoretestutil testoktetacore testpiecetablechangelib Qt5::Test KF5::I18n
)

########### swaprangespiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( swaprangespiecetablechangeabstractpiecetablechangeiftest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES oktetacoretestutil testoktetacore testpiecetablechangelib Qt5::Test KF5::I18n
)

########### grouppiecetablechangeabstractpiecetablechangeiftest ###############

# grouped changes break some assumptions and got workarounds, so disable for now
if(IWILLPICKUPWORKONOKTETACORE)
ecm_add_test( grouppiecetablechangeabstractpiecetablechangeiftest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES oktetacoretestutil testoktetacore testpiecetablechangelib Qt5::Test
)
endif()

########### grouppiecetablechangetest ###############

ecm_add_test( grouppiecetablechangetest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES testoktetacore testpiecetablechangelib Qt5::Test KF5::I18n
)

########### piecetablechangehistorytest ###############

ecm_add_test( piecetablechangehistorytest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES testoktetacore testpiecetablechangelib Qt5::Test
)

########### revertablepiecetabletest ###############

ecm_add_test( revertablepiecetabletest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES testoktetacore Qt5::Test KF5::I18n
)
