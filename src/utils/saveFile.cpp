#include "save_file.hpp"

void saveFile(const std::vector<std::byte>& data, const std::string& out)
{
    std::ofstream outFile(out, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Failed to open output file: " + out);
    }

    outFile.write(reinterpret_cast<const char*>(data.data()), data.size());
    outFile.close();
}