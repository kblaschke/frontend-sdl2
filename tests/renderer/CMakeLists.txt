# Requires gtest, otherwise fail
find_package(GTest 1.10 REQUIRED NO_MODULE)

# ===
# Integration Test
# ===
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


# ===
# Unit Test
# ===
# Unit test executable (fake RenderLoop via symbols in MainExecutionTest.cpp)
add_executable(projectMSDL-mainexec-test
    MainExecutionTest.cpp
)

target_link_libraries(projectMSDL-mainexec-test
    PRIVATE
    projectMSDL_testcorelib_norenderloop
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME projectMSDL-mainexec-test COMMAND projectMSDL-mainexec-test)