#include "CliOptions.hpp"

Owl::LogLevelValidator::LogLevelValidator() {
    name_ = "LogLevel";
    func_ = [](const std::string &str) {
        std::__1::set<std::string> levels(SPDLOG_LEVEL_NAMES);
        if (levels.contains(str)) {
            spdlog::info("Log Level: {}", str);
            return std::string();
        } else
            return fmt::format("Unacceptable log level: {}", str);
    };
}

/**
 * @brief Initialize cli parser and bind the cliOptionResults to app
 *
 * @param app CLI parser
 * @param cliOptionResults Options to store the parsed results
 *
 * @note You should invoke parse method like @c CLI11_PARSE(app, argc, argv) to parse fragments
 * after call this method
 */
void Owl::InitialCLI(CLI::App &app, Owl::CLIOptions &cliOptionResults) {
    app.add_option("-l,--log", cliOptionResults.logLevel,
                   fmt::format("Set log level in [{}]",
                               fmt::join(std::__1::vector<std::string>(SPDLOG_LEVEL_NAMES), ", ")))
            ->type_name("LEVEL")
            ->check(LogLevelValidator())
            ->default_val("info");

    app.add_option("-c,--config", cliOptionResults.config, "Set config path")
            ->type_name("PATH")
            ->required()
            ->check(CLI::ExistingFile);
}
