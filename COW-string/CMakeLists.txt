project(COW-string)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp)

enable_testing()

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(tests tests.cpp)

target_link_libraries(tests
 PRIVATE
  GTest::GTest)

add_test(multiply_gtests tests)