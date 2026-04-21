set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

# to see Okteta Core headers for Address, Size, etc. types
# also for tests to enable more namespaced <piecetable/*> includes
include_directories(
  ${SUTDIR}/..
)

add_library( testpiecetablechangelib STATIC
  abstractpiecetablechangeiftest.cpp
  testpiecetablechange.cpp
)
target_link_libraries( testpiecetablechangelib
  PUBLIC
    TestOktetaCore
    Qt5::Test
)

ecm_add_test( piecetest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES Qt5::Test
)

ecm_add_test(
  piecelisttest.cpp
  ${SUTDIR}/piecelist.cpp
  TEST_NAME piecelisttest
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES Qt5::Test
)

ecm_add_test( piecetabletest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES TestOktetaCore testpiecetablechangelib Qt5::Test
)

ecm_add_test( testpiecetablechangetest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES TestOktetaCoreTestUtil testpiecetablechangelib Qt5::Test
)

ecm_add_test( testpiecetablechangeabstractpiecetablechangeiftest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES TestOktetaCoreTestUtil TestOktetaCore testpiecetablechangelib Qt5::Test
)

ecm_add_test( insertpiecetablechangeabstractpiecetablechangeiftest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES TestOktetaCoreTestUtil TestOktetaCore testpiecetablechangelib Qt5::Test
)

ecm_add_test( removepiecetablechangeabstractpiecetablechangeiftest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES TestOktetaCoreTestUtil TestOktetaCore testpiecetablechangelib Qt5::Test
)

ecm_add_test( replacepiecetablechangeabstractpiecetablechangeiftest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES TestOktetaCoreTestUtil TestOktetaCore testpiecetablechangelib Qt5::Test
)

ecm_add_test( swaprangespiecetablechangeabstractpiecetablechangeiftest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES TestOktetaCoreTestUtil TestOktetaCore testpiecetablechangelib Qt5::Test
)

# grouped changes break some assumptions and got workarounds, so disable for now
if(IWILLPICKUPWORKONOKTETACORE)
ecm_add_test( grouppiecetablechangeabstractpiecetablechangeiftest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES TestOktetaCoreTestUtil TestOktetaCore testpiecetablechangelib Qt5::Test
)
endif()

ecm_add_test( grouppiecetablechangetest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES TestOktetaCore testpiecetablechangelib Qt5::Test
)

ecm_add_test( piecetablechangehistorytest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES TestOktetaCore testpiecetablechangelib Qt5::Test
)

ecm_add_test( revertablepiecetabletest.cpp
  NAME_PREFIX "libpiecetable-"
  LINK_LIBRARIES TestOktetaCore Qt5::Test
)
