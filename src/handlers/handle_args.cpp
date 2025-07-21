#include "handle_args.hpp"
#include "encrypt.hpp"

#include <iostream>

void handle_args(AppConfig config)
{
    switch (config.operation)
    {
    case Operation::Encrypt:
        encrypt(
            ask_password(),
            config
        );
        break;
    case Operation::Decrypt:
        break;
    default:
        break;
    }
}

std::string ask_password()
{
    
}