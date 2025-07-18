#include <iostream>
#include <CLI/CLI.hpp>
#include "app_config.hpp"

int main(int argc, char** argv)
{
    CLI::App app {"Crypto-Cpp APP"};
    argv = app.ensure_utf8(argv);

    AppConfig config;
    config.configure(app);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    std::cout<<"Worked: "<< config.input <<std::endl;

    return 0;
}