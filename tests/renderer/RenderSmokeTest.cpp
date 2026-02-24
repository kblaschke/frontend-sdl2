#include <gtest/gtest.h>
#include "ProjectMSDLApplication.h"
#include "SDLRenderingWindow.h"
#include "ProjectMWrapper.h"

#include <SDL2/SDL_opengl.h>
#include <vector>
#include <cstdlib>


// Helper function:
// Count how many pixels are NOT black
// Epsilon is our value for not black. Anything less than 2 on the GPU is black (some GPU's don't see 0,0,0 as black).
static size_t CountNonBlackPixels(const std::vector<unsigned char>& rgba, unsigned char epsilon = 2) {
    size_t count = 0;
    for (size_t i = 0; i + 2 < rgba.size(); i += 4) {
        if (rgba[i] > epsilon || rgba[i + 1] > epsilon || rgba[i + 2] > epsilon)
            ++count;
    }
    return count;
}

// Integration test:
// Verifies that:
// - App can create a window and OpenGL context
// - Renderer can draw something
// - Output is not just a blank screen
TEST(RenderSmokeTest, AppRendersSomething) {
// Tell SDL to use a dummy audio driver so audio init doesn't fail on Linux/MacOS
#if !defined(_WIN32)
    setenv("SDL_AUDIODRIVER", "dummy", 1);
#endif

    // Simulate starting the real app
    const char* argv0 = "projectMSDL";
    int argc = 1;
    char* argv[] = { const_cast<char*>(argv0), nullptr };

    // Create real app obj
    ProjectMSDLApplication app;

    // Init the app
    ASSERT_NO_THROW(app.init(argc, argv));
    
    // Create SDL window + OpenGL context
    auto& window = app.getSubsystem<SDLRenderingWindow>();
    ASSERT_NO_THROW(window.initialize(app));

    // Give SDL a moment / pump events
    SDL_PumpEvents();

    // Create the projectM renderer
    auto& renderer = app.getSubsystem<ProjectMWrapper>();
    ASSERT_NO_THROW(renderer.initialize(app));

    // Get the size of the drawable OpenGL area
    int width = 0, height = 0;
    window.GetDrawableSize(width, height);

    // Fail if H/W are 0
    ASSERT_GT(width, 0);
    ASSERT_GT(height, 0);

    // Render a few frames
    for (int i = 0; i < 3; ++i) {
        renderer.RenderFrame();
        glFinish();
    }

    // Prepare buffer and read pixels
    const size_t bufSize = static_cast<size_t>(width) * static_cast<size_t>(height) * 4;
    std::vector<unsigned char> pixels(bufSize);

    glPixelStorei(GL_PACK_ALIGNMENT, 1); // safe alignment for glReadPixels

    // Try reading back from the back buffer first
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glFinish();

    // Count non black pixels in back buffer
    size_t nonBlack = CountNonBlackPixels(pixels);

    // Threshold: at least 100 pixels OR at least 0.2% of the image
    size_t threshold = static_cast<size_t>(width) * static_cast<size_t>(height) / 500;
    if (threshold < 100) threshold = 100;

    EXPECT_GT(nonBlack, threshold);

    // Clean up
    renderer.uninitialize();
    window.uninitialize();
}