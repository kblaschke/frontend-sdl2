find_package(GTest 1.10 REQUIRED NO_MODULE)

add_executable(projectMSDL-audio-tests
    ListAudioDevicesTest.cpp
)

target_include_directories(projectMSDL-audio-tests 
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(projectMSDL-audio-tests
    PRIVATE
    projectMSDL_testcorelib
    GTest::gtest
    GTest::gtest_main
)

add_test(NAME projectMSDL-audio-tests COMMAND projectMSDL-audio-tests)