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

    EXPECT_FALSE(cfg->getBool("audio.listDevices", false));

    // Call the option handler directly (name and value are unused in implementation).
    app.ListAudioDevices("", "");

    // Check to see if config has been correctly overridden
    EXPECT_TRUE(cfg->getBool("audio.listDevices", false));

    // Get config again, to test using a fresh config object
    auto cfg = app.CommandLineConfiguration();
    ASSERT_TRUE(cfg); // check configuration object exists

    EXPECT_TRUE(cfg->getBool("audio.listDevices", false));
}