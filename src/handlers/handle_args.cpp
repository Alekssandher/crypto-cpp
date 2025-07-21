#include "handle_args.hpp"
#include "encrypt.hpp"
#include "decrypt.hpp"

#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

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
        decrypt(
            ask_password(),
            config
        );
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

    #ifdef _WIN32
        // Hide input on windows
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        GetConsoleMode(hStdin, &mode);
        SetConsoleMode(hStdin, mode & ~ENABLE_ECHO_INPUT);

        std::getline(std::cin, password);

        SetConsoleMode(hStdin, mode);
    #else
        // Hide input on linux
        termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        std::getline(std::cin, password);

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    #endif

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