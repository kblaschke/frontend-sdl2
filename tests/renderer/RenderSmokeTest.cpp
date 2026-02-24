#include <gtest/gtest.h>
#include "ProjectMSDLApplication.h"
#include "SDLRenderingWindow.h"
#include "ProjectMWrapper.h"

#if defined(_WIN32)
    #include <GL/glew.h>
#endif

#include <SDL2/SDL_opengl.h>
#include <vector>
#include <cstdlib>

struct RendererSmokeCleanup {
    ProjectMWrapper* renderer = nullptr;
    SDLRenderingWindow* window = nullptr;

    ~RendererSmokeCleanup() {
        if (renderer) renderer->uninitialize();
        if (window) window->uninitialize();
    }
};

auto CheckGLError = [] (const char* where) {
    GLenum err = glGetError();
    ASSERT_EQ(err, GL_NO_ERROR) << "OpenGL error at " << where << ": 0x" << std::hex << err;
};

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

    // Give SDL a moment / pump events
    SDL_PumpEvents();

    // Create the projectM renderer
    auto& renderer = app.getSubsystem<ProjectMWrapper>();

    // Create a cleaner
    RendererSmokeCleanup cleanup;
    cleanup.window = &window;
    cleanup.renderer = &renderer;

    // Make sure these actually initialized correctly
    ASSERT_NO_THROW(window.initialize(app));
    ASSERT_NO_THROW(renderer.initialize(app));

    // Get the size of the drawable OpenGL area
    int width = 0, height = 0;
    window.GetDrawableSize(width, height);

    // Fail if H/W are 0
    ASSERT_GT(width, 0);
    ASSERT_GT(height, 0);

    // Make sure our OpenGL window is clear first
    glViewport(0, 0, width, height);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glFinish();
    CheckGLError("ClearColor");

    // Render a few frames
    for (int i = 0; i < 20; ++i) {
        renderer.RenderFrame();
        glFinish();
        CheckGLError("RenderFrame");
    }

    // Prepare buffer and read pixels
    const size_t bufSize = static_cast<size_t>(width) * static_cast<size_t>(height) * 4;
    std::vector<unsigned char> pixels(bufSize);
    glPixelStorei(GL_PACK_ALIGNMENT, 1); // safe alignment for glReadPixels

    // Ensure we're bound to the default framebuffer because the renderer may have left an FBO bound
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CheckGLError("glBindFramebuffer(0)");

    // Pick a read buffer that actually exists (GL_BACK if double-buffered; otherwise GL_FRONT)
    int doubleBuffered = 0;
    SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &doubleBuffered);
    glReadBuffer(doubleBuffered ? GL_BACK : GL_FRONT);
    CheckGLError("glReadBuffer");

    // Read pixels from the chosen buffer
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    glFinish();
    CheckGLError("glReadPixels");

    // Count non black pixels in back buffer
    size_t nonBlack = CountNonBlackPixels(pixels);

    // Threshold: at least 100 pixels OR at least 0.2% of the image
    size_t threshold = static_cast<size_t>(width) * static_cast<size_t>(height) / 500;
    if (threshold < 100) threshold = 100;

    EXPECT_GT(nonBlack, threshold);
}