# ======
# Test-only core library
# (Eventually this should be moved into src/CMakeLists.txt to avoid
# duplicating source lists and dependancies.)
# ======

# Build a core library from production sources to test on
add_library(projectMSDL_testcorelib
    # Add .cpp files as needed for testing
    # Examples:
    # ${PROJECT_SOURCE_DIR}/src/SDLRenderingWindow.cpp
    # ${PROJECT_SOURCE_DIR}/src/RenderLoop.cpp
    # ${PROJECT_SOURCE_DIR}/src/ProjectMWrapper.cpp
    # For now, we'll include a simple file just so this thing works. It can be removed if unneeded later.
    ${PROJECT_SOURCE_DIR}/src/FPSLimiter.cpp
    ${PROJECT_SOURCE_DIR}/src/ProjectMSDLApplication.cpp
    ${PROJECT_SOURCE_DIR}/src/RenderLoop.cpp
    ${PROJECT_SOURCE_DIR}/src/ProjectMWrapper.cpp
    ${PROJECT_SOURCE_DIR}/src/SDLRenderingWindow.cpp
    ${PROJECT_SOURCE_DIR}/src/AudioCapture.cpp
)

if (WIN32)
    target_sources(projectMSDL_testcorelib PRIVATE
        ${PROJECT_SOURCE_DIR}/src/AudioCaptureImpl_WASAPI.cpp
    )
    target_compile_definitions(projectMSDL_testcorelib PRIVATE
        AUDIO_IMPL_HEADER="AudioCaptureImpl_WASAPI.h"
        USE_GLEW
    )
else()
    target_sources(projectMSDL_testcorelib PRIVATE
        ${PROJECT_SOURCE_DIR}/src/AudioCaptureImpl_SDL.cpp
    )
    target_compile_definitions(projectMSDL_testcorelib PRIVATE
        AUDIO_IMPL_HEADER="AudioCaptureImpl_SDL.h"
    )
endif()

# Headers from the production source tree
target_include_directories(projectMSDL_testcorelib
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)

# Compile definitions that are commonly referenced
target_compile_definitions(projectMSDL_testcorelib
    PRIVATE
    PROJECTMSDL_CONFIG_LOCATION="${DEFAULT_CONFIG_PATH}"
    PROJECTMSDL_VERSION="${PROJECT_VERSION}"
)

# Linker set (add to this only when the linker asks)
target_link_libraries(projectMSDL_testcorelib
    PRIVATE
    SDL2::SDL2
    Poco::Util
    Poco::Foundation
    ProjectMSDL-GUI
    ProjectMSDL-Notifications
    libprojectM::playlist
)

# ======
# Actual test subdirs 
# Add to this when defining more testing directories
# (To be kept in this file after integration of above code into src/CMakeLists.txt)
# ======
add_subdirectory(renderer)
add_subdirectory(audio)