// HelpOptionTest.cpp
//
// CLI integration test: verifies `projectMSDL --help` prints usage/help text
// and exits successfully without entering ProjectMSDLApplication::main()
// or starting RenderLoop::Run().

#include <gtest/gtest.h>

// Poco: process launching, pipes, timing, filesystem
#include <Poco/Exception.h>
#include <Poco/File.h>
#include <Poco/Pipe.h>
#include <Poco/PipeStream.h>
#include <Poco/Process.h>
#include <Poco/StreamCopier.h>
#include <Poco/Thread.h>
#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>

#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifndef PROJECTMSDL_BINARY_PATH
#error "PROJECTMSDL_BINARY_PATH is not defined. Check tests/cli/CMakeLists.txt."
#endif

// Read entire contents of a pipe into a string.
static std::string ReadAll(Poco::Pipe& pipe)
{
    Poco::PipeInputStream is(pipe);
    std::ostringstream oss;
    Poco::StreamCopier::copyStream(is, oss);
    return oss.str();
}

// Result of running the CLI.
struct CliResult
{
    int exitCode;
    std::string output; // stdout + stderr combined
};

// Helper: launches projectMSDL with given args.
// Throws std::runtime_error on failure.
static CliResult RunCli(const std::vector<std::string>& args, int timeoutSeconds = 3)
{
    const std::string binaryPath = PROJECTMSDL_BINARY_PATH;

    // Ensure binary exists and is executable.
    Poco::File binFile(binaryPath);
    if (!binFile.exists()) throw std::runtime_error("Binary not found: " + binaryPath);
    if (!binFile.canExecute()) throw std::runtime_error("Binary not executable: " + binaryPath);

    // Capture stdout/stderr.
    Poco::Pipe outPipe;
    Poco::Pipe errPipe;

    // Launch the process and construct the ProcessHandle from the return value.
    Poco::ProcessHandle handle = Poco::Process::launch(binaryPath, args, nullptr, &outPipe, &errPipe);

    // --- Timeout guard (detect accidental RenderLoop entry) ---
    const Poco::Timespan timeout(timeoutSeconds, 0);
    Poco::Timestamp start;
    int exitCode = -1;

    while (true)
    {
        const int maybeExit = handle.tryWait(); // -1 if still running
        if (maybeExit != -1)
        {
            exitCode = maybeExit;
            break;
        }

        if (start.elapsed() > timeout.totalMicroseconds())
        {
            try { Poco::Process::kill(handle); } catch (...) {}
            throw std::runtime_error("CLI process did not exit before timeout (possible RenderLoop entry).");
        }

        Poco::Thread::sleep(10);
    }

    // Read output after exit.
    std::string stdoutText;
    std::string stderrText;

    try { stdoutText = ReadAll(outPipe); }
    catch (const Poco::Exception& ex)
    {
        throw std::runtime_error(std::string("Error reading stdout: ") + ex.displayText());
    }

    try { stderrText = ReadAll(errPipe); }
    catch (const Poco::Exception& ex)
    {
        throw std::runtime_error(std::string("Error reading stderr: ") + ex.displayText());
    }

    return { exitCode, stdoutText + (stderrText.empty() ? "" : "\n" + stderrText) };
}

TEST(CliHelpOption, HelpPrintsAndExitsWithoutRunning)
{
    CliResult result;

    try
    {
#ifdef _WIN32
        result = RunCli({ "/help" });
#else
        result = RunCli({ "--help" });
#endif
    }
    catch (const std::exception& ex)
    {
        FAIL() << "RunCli failed: " << ex.what();
        return;
    }

    // Must exit successfully.
    ASSERT_EQ(result.exitCode, 0) << "Non-zero exit (" << result.exitCode << "). Output:\n" << result.output;

    // Must print recognizable help/usage text.
    EXPECT_NE(result.output.find("projectM SDL Standalone Visualizer"), std::string::npos) << "Expected help header not found.\nOutput:\n" << result.output;
    EXPECT_NE(result.output.find("[options]"), std::string::npos) << "Expected usage/options text not found.\nOutput:\n" << result.output;
}