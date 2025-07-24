#include <iostream>
#include <CLI/CLI.hpp>
#include "app_config.hpp"
#include "handle_args.hpp"
#include "sodium.h"

int main(int argc, char** argv)
{
    if (sodium_init() == -1) {
        std::cerr << "libsodium failed to initialize.\n";
        return 1;
    }
    
    CLI::App app {"crypto-cpp APP"};
    
    argv = app.ensure_utf8(argv);

    AppConfig config;
    config.configure(app);

    try {
        app.parse(argc, argv);
        handle_args(config);
        return 0;
        
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    std::cout<<"Worked: "<< config.input <<std::endl;

    return 0;
}
