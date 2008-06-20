cmake_minimum_required(VERSION 2.4)

project(Util--)
SET(PROJECT_VERSION 1.0rc1)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
FIND_PACKAGE(Boost REQUIRED COMPONENTS graph filesystem thread regex)

include_directories(${CMAKE_BINARY_DIR}) # to get cmake-generated utilmm/config/config.h
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIR})
add_subdirectory (src)

INCLUDE(TestBigEndian)
TEST_BIG_ENDIAN(WORDS_BIGENDIAN)
CONFIGURE_FILE(utilmm/config/config.h.in utilmm/config/config.h)
CONFIGURE_FILE(utilmm.pc.in utilmm.pc @ONLY)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/utilmm.pc 
    DESTINATION lib/pkgconfig)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/utilmm/config/config.h 
    DESTINATION include/utilmm/config)
INSTALL(DIRECTORY utilmm/ DESTINATION include/utilmm
    PATTERN "*.in" EXCLUDE)

INCLUDE(CTest)
FIND_PACKAGE(Boost COMPONENTS unit_test_framework)
IF (Boost_UNIT_TEST_FRAMEWORK_FOUND)
    MESSAGE(STATUS "boost/test found ... building test suite")
    ADD_SUBDIRECTORY(test)
ENDIF(Boost_UNIT_TEST_FRAMEWORK_FOUND)

INCLUDE(GenerateDoxygenDoc)
