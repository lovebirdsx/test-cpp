enable_testing()

add_executable(TestApp test_app.cpp)
target_include_directories(TestApp PRIVATE ${CMAKE_SOURCE_DIR}/third_party/doctest)
target_link_libraries(TestApp PRIVATE TestCppLib)

add_test(NAME TestApp COMMAND TestApp)
