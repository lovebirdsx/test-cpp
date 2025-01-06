enable_testing()

file(GLOB TEST_SOURCES "*.cpp" "stl/*.cpp")
add_executable(TestApp ${TEST_SOURCES})
target_include_directories(TestApp PRIVATE ${CMAKE_SOURCE_DIR}/third_party/doctest)
target_link_libraries(TestApp PRIVATE TestCppLib)

add_test(NAME TestApp COMMAND TestApp)
