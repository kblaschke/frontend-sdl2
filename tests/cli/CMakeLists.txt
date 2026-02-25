find_package(GTest 1.10 REQUIRED NO_MODULE)

add_executable(projectMSDL-cli-test
    HelpOptionTest.cpp
)

target_link_libraries(projectMSDL-cli-test
    PRIVATE
    GTest::gtest_main
    Poco::Foundation
)

# Ensure the app is built before the test runs
add_dependencies(projectMSDL-cli-test projectMSDL)

# Pass the built binary path into the test as a compile definition
target_compile_definitions(projectMSDL-cli-test
    PRIVATE
    PROJECTMSDL_BINARY_PATH="$<TARGET_FILE:projectMSDL>"
)

add_test(NAME projectMSDL-cli-test COMMAND $<TARGET_FILE:projectMSDL-cli-test>)