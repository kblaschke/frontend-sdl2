# Requires gtest, otherwise fail
find_package(GTest 1.10 REQUIRED NO_MODULE)

# Creation of executable for this specific test
add_executable(projectMSDL-renderer-test
    RenderSmokeTest.cpp
)

# Call required libraries; testcore, gtest
target_link_libraries(projectMSDL-renderer-test
    PRIVATE
    projectMSDL_testcorelib
    GTest::gtest
    GTest::gtest_main
)

# Add our test to the executable
add_test(NAME projectMSDL-renderer-test COMMAND projectMSDL-renderer-test)