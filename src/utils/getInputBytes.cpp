#include <get_input_bytes.hpp>

#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using std::ifstream;
using std::vector;

std::vector<std::byte> getInputBytes(const std::string& i)
{

    // Opening the file from input 

    std::ifstream file(i, std::ios::binary | std::ios::ate);
    
    if(!file)
    {
        throw std::runtime_error("Error opening file: " + i);
    }

    // Get the size of the file by using a pointer

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Buffer to store the file
    vector<std::byte> buffer(size);

    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Error reading file: " + i);
    }

    return buffer;
}