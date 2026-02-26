#define SDL_MAIN_HANDLED 1

#include <gtest/gtest.h>

#include <cstdlib>
#include <vector>
#include <string>

#include <Poco/Util/Application.h>
#include <Poco/AutoPtr.h>

#include "ProjectMSDLApplication.h"
#include "RenderLoop.h"

#include "AudioCapture.h"
#include "ProjectMWrapper.h"
#include "SDLRenderingWindow.h"
#include "gui/ProjectMGUI.h"
#include "notifications/QuitNotification.h"

static int g_run_calls = 0;

// RenderLoop.cpp is excluded from projectMSDL_testcorelib_norenderloop so we provide these symbols here.
RenderLoop::RenderLoop()
    : _audioCapture(Poco::Util::Application::instance().getSubsystem<AudioCapture>())
    , _projectMWrapper(Poco::Util::Application::instance().getSubsystem<ProjectMWrapper>())
    , _sdlRenderingWindow(Poco::Util::Application::instance().getSubsystem<SDLRenderingWindow>())
    , _projectMGui(Poco::Util::Application::instance().getSubsystem<ProjectMGUI>())
{
}

// RenderLoop has an observer bound to this method.
void RenderLoop::QuitNotificationHandler(const Poco::AutoPtr<QuitNotification>&)
{
}

void RenderLoop::Run()
{
    ++g_run_calls;
}

class TestableProjectMSDLApplication final : public ProjectMSDLApplication {
public:
    using ProjectMSDLApplication::main;
};

TEST(ProjectMSDLApplicationMain, CallsRenderLoopRunExactlyOnceAndReturnsExitSuccess)
{
    g_run_calls = 0;

    TestableProjectMSDLApplication app;
    const int rc = app.main(std::vector<std::string>{});

    EXPECT_EQ(g_run_calls, 1);
    EXPECT_EQ(rc, EXIT_SUCCESS);
}