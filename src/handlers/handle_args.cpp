#include "handle_args.hpp"
#include "encrypt.hpp"
#include "decrypt.hpp"
#include "delete_original_file.hpp"

#include <iostream>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

constexpr size_t MAX_PASSWORD_LENGTH = 128;

void clear_terminal() 
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

std::string ask_password(const std::string& prompt = "Type a password: ") {
    std::string password;
    password.reserve(128); 
    std::cout << prompt;

#ifdef _WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & ~ENABLE_ECHO_INPUT);

    std::getline(std::cin, password);

    SetConsoleMode(hStdin, mode);
#else
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::getline(std::cin, password);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    clear_terminal();
    return password;
}

std::string ask_password_twice() {
    std::string password = ask_password();
    std::string password_confirm;

    while (true) {
        password_confirm = ask_password("Retype password: ");
        if (password == password_confirm) break;

        clear_terminal();
        std::cout << "The passwords don't match!" << std::endl;

        std::fill(password_confirm.begin(), password_confirm.end(), 0);

        password = ask_password(); 
    }

    std::fill(password_confirm.begin(), password_confirm.end(), 0);

    return password;
}

void handle_args(AppConfig config) {
    
    switch (config.operation) {
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

    if (config.deleteOriginalFile) {
        deleteOriginalFile(config.input, config.deleteOverwriteTimes);
    }
}
