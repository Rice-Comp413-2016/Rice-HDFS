# Locate GTest
ADD_SUBDIRECTORY(/usr/src/gtest gtest)
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Create an executable called runTests with the test file you want to run
file(GLOB test_sources
	"*.cc"
)
#set(test_sources
#	tests.cc
#	testOutHere.cc
#)
add_executable(sample_tests ${test_sources})
target_link_libraries(sample_tests gtest_main)
