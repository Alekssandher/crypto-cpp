#pragma once
#include <CLI/CLI.hpp>
#include <iostream>

enum class Operation {
    Encrypt,
    Decrypt
};

struct AppConfig {
    std::string input;
    std::string output;
    Operation operation;

    unsigned int iterations;
    bool verbose;
    bool deleteOriginalFile;
    int deleteOverwriteTimes;
    void configure(CLI::App &app);
};