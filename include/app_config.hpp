#pragma once
#include <CLI/CLI.hpp>
#include <iostream>

struct AppConfig {
    std::string input;
    std::string output;
    std::string operation;

    unsigned int iterations;
    bool verbose;
    bool overwrite;
    
    void configure(CLI::App &app);
};