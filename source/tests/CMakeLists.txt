find_package(GTest CONFIG REQUIRED)

add_executable(material_graph_test material_graph_test.cpp)
target_link_libraries(material_graph_test  GTest::gtest GTest::gtest_main afro)

add_executable(undo_test undo_test.cpp)
target_link_libraries(undo_test  GTest::gtest GTest::gtest_main afro)

include(GoogleTest)
gtest_discover_tests(material_graph_test)
gtest_discover_tests(undo_test)

add_custom_target(tests)

add_dependencies(tests material_graph_test)
add_dependencies(tests undo_test)
