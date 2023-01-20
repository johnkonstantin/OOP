project(Google_tests)

add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

cmake_path(GET CMAKE_CURRENT_SOURCE_DIR PARENT_PATH TEST_DIR)
set(TEST_DIR "${TEST_DIR}/Tests")
add_executable(Google_Tests_run "${TEST_DIR}/tests.cpp")
target_link_libraries(Google_Tests_run gtest gtest_main)
target_link_libraries(Google_Tests_run iterator_lib)