file(GLOB LIB_SOURCES "lib/*.cpp")

add_library(TestCppLib STATIC
    ${LIB_SOURCES}
)

target_include_directories(TestCppLib PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

add_executable(TestCpp main.cpp)

target_link_libraries(TestCpp PRIVATE TestCppLib)
