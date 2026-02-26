// tests/ui/EscOverlayToggleTest.cpp

#include <gtest/gtest.h>

#include "ProjectMSDLApplication.h"
#include "RenderLoop.h"
#include "SDLRenderingWindow.h"
#include "ProjectMWrapper.h"
#include "gui/ProjectMGUI.h"

#include <SDL2/SDL.h>
#include <imgui.h>

#include <cstdlib>
#include <chrono>
#include <thread>

namespace {

#if !defined(_WIN32)
void ConfigureHeadlessIfNeeded()
{
#if defined(__linux__)
    const char* display = std::getenv("DISPLAY");
    if (!display || display[0] == '\0')
    {
        if (!std::getenv("SDL_VIDEODRIVER"))
            setenv("SDL_VIDEODRIVER", "dummy", 1);
    }
#endif
    if (!std::getenv("SDL_AUDIODRIVER"))
        setenv("SDL_AUDIODRIVER", "dummy", 1);
}
#endif

class RenderLoopTestHarness : public RenderLoop
{
public:
    void PollEventsPublic() { PollEvents(); }
    bool WantsToQuitPublic() const { return _wantsToQuit; }
};

void PumpOnce(ProjectMGUI& gui, RenderLoopTestHarness& loop)
{
    loop.PollEventsPublic();
    gui.Draw();
}

void PushEscPress(SDL_Window* w)
{
    const Uint32 wid = w ? SDL_GetWindowID(w) : 0;

    SDL_Event e{};
    e.type = SDL_KEYDOWN;
    e.key.type = SDL_KEYDOWN;
    e.key.windowID = wid;
    e.key.state = SDL_PRESSED;
    e.key.repeat = 0;
    e.key.keysym.sym = SDLK_ESCAPE;
    e.key.keysym.scancode = SDL_SCANCODE_ESCAPE;
    e.key.keysym.mod = KMOD_NONE;
    ASSERT_EQ(SDL_PushEvent(&e), 1);

    e = SDL_Event{};
    e.type = SDL_KEYUP;
    e.key.type = SDL_KEYUP;
    e.key.windowID = wid;
    e.key.state = SDL_RELEASED;
    e.key.repeat = 0;
    e.key.keysym.sym = SDLK_ESCAPE;
    e.key.keysym.scancode = SDL_SCANCODE_ESCAPE;
    e.key.keysym.mod = KMOD_NONE;
    ASSERT_EQ(SDL_PushEvent(&e), 1);
}

void PushBackgroundClick(SDL_Window* w, int x, int y)
{
    if (!w) return;
    const Uint32 wid = SDL_GetWindowID(w);

    SDL_Event e{};
    e.type = SDL_MOUSEMOTION;
    e.motion.windowID = wid;
    e.motion.which = 0;
    e.motion.x = x;
    e.motion.y = y;
    e.motion.xrel = 0;
    e.motion.yrel = 0;
    SDL_PushEvent(&e);

    e = SDL_Event{};
    e.type = SDL_MOUSEBUTTONDOWN;
    e.button.windowID = wid;
    e.button.which = 0;
    e.button.button = SDL_BUTTON_LEFT;
    e.button.state = SDL_PRESSED;
    e.button.clicks = 1;
    e.button.x = x;
    e.button.y = y;
    SDL_PushEvent(&e);

    e = SDL_Event{};
    e.type = SDL_MOUSEBUTTONUP;
    e.button.windowID = wid;
    e.button.which = 0;
    e.button.button = SDL_BUTTON_LEFT;
    e.button.state = SDL_RELEASED;
    e.button.clicks = 1;
    e.button.x = x;
    e.button.y = y;
    SDL_PushEvent(&e);
}

bool WaitForImGuiReady(ProjectMGUI& gui, RenderLoopTestHarness& loop, int maxFrames)
{
    for (int i = 0; i < maxFrames; ++i)
    {
        PumpOnce(gui, loop);
        if (ImGui::GetCurrentContext() != nullptr)
            return true;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    return false;
}

bool WaitForImGuiNotCapturingKeyboard(ProjectMGUI& gui, RenderLoopTestHarness& loop, int maxFrames)
{
    for (int i = 0; i < maxFrames; ++i)
    {
        PumpOnce(gui, loop);

        if (ImGui::GetCurrentContext() == nullptr)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            continue;
        }

        const ImGuiIO& io = ImGui::GetIO();
        if (!io.WantCaptureKeyboard)
            return true;

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    return false;
}

} // namespace

TEST(UIToggleOverlayTest, EscTogglesOverlayVisibilityOncePerPress)
{
#if !defined(_WIN32)
    ConfigureHeadlessIfNeeded();
#endif

    const char* argv0 = "projectMSDL-ui-test";
    int argc = 1;
    char* argv[] = { const_cast<char*>(argv0), nullptr };

    ProjectMSDLApplication app;
    ASSERT_NO_THROW(app.init(argc, argv));

    auto& window = app.getSubsystem<SDLRenderingWindow>();
    auto& renderer = app.getSubsystem<ProjectMWrapper>();
    auto& gui = app.getSubsystem<ProjectMGUI>();

    ASSERT_NO_THROW(window.initialize(app));
    ASSERT_NO_THROW(renderer.initialize(app));
    ASSERT_NO_THROW(gui.initialize(app));

    SDL_Window* sdlWin = window.GetRenderingWindow();
    ASSERT_NE(sdlWin, nullptr);

    RenderLoopTestHarness loop;

    ASSERT_TRUE(WaitForImGuiReady(gui, loop, 60)) << "ImGui context never became ready.";

    // Try to ensure no widget is focused/active by clicking the far bottom-right.
    int w = 0, h = 0;
    SDL_GetWindowSize(sdlWin, &w, &h);
    PushBackgroundClick(sdlWin, (w > 0 ? w - 2 : 1), (h > 0 ? h - 2 : 1));

    // Wait until ImGui naturally reports it is NOT capturing keyboard.
    ASSERT_TRUE(WaitForImGuiNotCapturingKeyboard(gui, loop, 120))
        << "ImGui kept WantCaptureKeyboard=true. "
        << "This can happen depending on UI state; test may be flaky on some platforms.";

    const bool initialVisible = gui.Visible();

    // Press ESC once -> should flip
    PushEscPress(sdlWin);
    PumpOnce(gui, loop);

    EXPECT_EQ(gui.Visible(), !initialVisible);
    EXPECT_FALSE(loop.WantsToQuitPublic());

    // Again: click background + wait for no-capture before the second press
    SDL_GetWindowSize(sdlWin, &w, &h);
    PushBackgroundClick(sdlWin, (w > 0 ? w - 2 : 1), (h > 0 ? h - 2 : 1));

    ASSERT_TRUE(WaitForImGuiNotCapturingKeyboard(gui, loop, 120))
        << "ImGui kept WantCaptureKeyboard=true before second ESC press.";

    // Press ESC again -> should restore
    PushEscPress(sdlWin);
    PumpOnce(gui, loop);

    EXPECT_EQ(gui.Visible(), initialVisible);
    EXPECT_FALSE(loop.WantsToQuitPublic());

    gui.uninitialize();
    renderer.uninitialize();
    window.uninitialize();
}