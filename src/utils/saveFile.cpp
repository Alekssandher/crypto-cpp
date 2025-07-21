#include "save_file.hpp"

void saveFile(const std::string& result, std::string out)
{
    std::ofstream outFile(out, std::ios::binary);
    outFile.write(result.data(), result.size());
    outFile.close();

}