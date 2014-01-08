ADD_EXECUTABLE(utilmm_testsuite
    test_configfile.cc test_misc.cc test_pkgconfig.cc
    test_process.cc test_shellexpand.cc testsuite.cc
    test_system.cc test_undirected_graph.cc)

TARGET_LINK_LIBRARIES(utilmm_testsuite utilmm
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES} ${Boost_SYSTEM_LIBRARIES})
ADD_TEST(Suite ${CMAKE_CURRENT_BINARY_DIR}/utilmm_testsuite --catch_system_errors=no)

