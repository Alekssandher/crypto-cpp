#include "delete_original_file.hpp"

#include <filesystem>
#include <fstream>
#include <random>
#include <cstdio>

void deleteOriginalFile(std::string &input, int &deleteTimes)
{
    namespace fs = std::filesystem;

    if(!fs::exists(input))
    {
        throw std::runtime_error("The file provided to delete no long exists.");
        return;
    }
    std::uintmax_t filesize = fs::file_size(input);

    std::fstream file(input, std::ios::in | std::ios::out | std::ios::binary);

    if(!file)
    {
        throw std::runtime_error("Could not open file to delete.");
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned char> dist(0, 255);

    std::vector<char> buffer(filesize);

    for (int i = 0; i < deleteTimes; ++i) {
        for (auto& byte : buffer) {
            byte = static_cast<char>(dist(gen));
        }

        file.seekp(0);
        file.write(buffer.data(), buffer.size());
        file.flush();
    }

    std::fill(buffer.begin(), buffer.end(), 0);
    file.seekp(0);
    file.write(buffer.data(), buffer.size());
    file.flush();

    file.close();

    
    if (std::remove(input.c_str()) != 0) {
        std::cerr << "Error Deleting File.\n";
    } else {
        std::cout << "File Overwritten Successfully.\n";
    }
}