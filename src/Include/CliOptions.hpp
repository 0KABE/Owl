#pragma once

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>

namespace Owl {
    struct CLIOptions {
        std::string logLevel;
        std::string config;
    };

    struct LogLevelValidator : public CLI::Validator {
        LogLevelValidator();
    };

    void InitialCLI(CLI::App &app, CLIOptions &cliOptionResults);
}


