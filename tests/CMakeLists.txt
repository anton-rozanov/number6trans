find_package(GTest REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)


add_executable(tests test_main.cpp lib_test.cpp)
target_link_libraries(tests PRIVATE GTest::GTest GTest::Main trans_lib)

add_test(NAME your_project_tests COMMAND tests)

