# Requires gtest, otherwise fail
find_package(GTest 1.10 REQUIRED NO_MODULE)

# Creation of executable for this specific test
add_executable(projectMSDL-ui-test
    EscOverlayToggleTest.cpp
)

# Call required libraries
target_link_libraries(projectMSDL-ui-test
    PRIVATE
    ImGui
    projectMSDL_testcorelib
    GTest::gtest_main
)

# Add our test to the executable
add_test(NAME projectMSDL-ui-test COMMAND projectMSDL-ui-test)