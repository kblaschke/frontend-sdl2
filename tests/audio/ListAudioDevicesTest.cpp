#include <gtest/gtest.h>
#include "ProjectMSDLApplication.h"

// Expose the protected ListAudioDevices for testing by deriving a small test subclass.
class TestableProjectMSDLApplication : public ProjectMSDLApplication {
public:
    using ProjectMSDLApplication::ListAudioDevices; // make protected method public in test subclass
};

TEST(ListAudioDevicesTest, SetsAudioListDevicesOverrideToTrue)
{
    TestableProjectMSDLApplication app;

    auto cfg = app.CommandLineConfiguration();
    ASSERT_TRUE(cfg); // check configuration object exists

    // Call the option handler directly (name and value are unused in implementation).
    app.ListAudioDevices("", "");

    EXPECT_TRUE(cfg->getBool("audio.listDevices", false));
}