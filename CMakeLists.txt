project(Util--)

set (CMAKE_MODULE_PATH "cmake")
FIND_PACKAGE(Boost REQUIRED COMPONENTS graph filesystem thread regex)

include_directories(/home/joyeux/dfki/dev/utilmm)
include_directories(${Boost_INCLUDE_DIR})
add_subdirectory (src)

INCLUDE(TestBigEndian)
TEST_BIG_ENDIAN(WORDS_BIGENDIAN)
CONFIGURE_FILE(utilmm/config/config.h.in utilmm/config/config.h)
CONFIGURE_FILE(utilmm.pc.in utilmm.pc @ONLY)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/utilmm.pc DESTINATION lib/pkgconfig)
INSTALL(DIRECTORY utilmm/ DESTINATION include
    PATTERN "*.in" EXCLUDE)


