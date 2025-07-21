#include "handle_args.hpp"
#include "encrypt.hpp"

#include <iostream>
#include <termios.h>
#include <unistd.h>

std::string ask_password(const std::string& prompt = "Type a password: ");
std::string ask_password_twice();
void clear_terminal();

void handle_args(AppConfig config)
{
    switch (config.operation)
    {
    case Operation::Encrypt:
        encrypt(
            ask_password_twice(),
            config
        );
        break;
    case Operation::Decrypt:
        break;
    default:
        break;
    }
}
std::string ask_password_twice()
{
    std::string password = ask_password();
    while(password != ask_password("Retype password: "))
    {
        clear_terminal();
        std::cout << "The passwords don't match!"<<std::endl;
        password = ask_password();
    }

    return password;

}
std::string ask_password(const std::string& prompt)
{
    std::string password;
    std::cout << prompt;
    termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~ECHO;

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::getline(std::cin, password);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;

    return password;
}

void clear_terminal() 
{
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}