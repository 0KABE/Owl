#pragma once

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>
#include "Util/Singleton.hpp"

namespace Owl {
    struct CLIOptions : public Singleton<CLIOptions> {
        friend Singleton<CLIOptions>;

        std::string logLevel;
        std::string config;
    private:
        CLIOptions() = default;
    };

    struct LogLevelValidator : public CLI::Validator {
        LogLevelValidator();
    };

    void InitialCLI(CLI::App &app, CLIOptions &cliOptionResults);
}


