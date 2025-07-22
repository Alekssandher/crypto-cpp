#pragma once
#include <CLI/CLI.hpp>
#include <iostream>
#include <sodium.h>

enum class Operation {
    Encrypt,
    Decrypt
};

struct AppConfig {
    std::string input;
    std::string output;
    Operation operation;

    int security_level_int;
    bool verbose = false;
    bool deleteOriginalFile = false;
    int deleteOverwriteTimes;
    void configure(CLI::App &app);

    static const std::map<std::string, Operation> operation_map;

    unsigned long long getOpsLimit()
    {
        switch (security_level_int) {
            case 1: return crypto_pwhash_OPSLIMIT_INTERACTIVE;
            case 3: return crypto_pwhash_OPSLIMIT_SENSITIVE;
            default: return crypto_pwhash_OPSLIMIT_MODERATE;
        }
    }

    size_t getMemLimit()
    {
        switch (security_level_int) {
            case 1: return crypto_pwhash_MEMLIMIT_INTERACTIVE;
            case 3: return crypto_pwhash_MEMLIMIT_SENSITIVE;
            default: return crypto_pwhash_MEMLIMIT_MODERATE;
        }
    }
};

