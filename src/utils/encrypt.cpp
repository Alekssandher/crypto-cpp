#include "encrypt.hpp"
#include "get_input_bytes.hpp"

#include <bits/stdc++.h>
#include <vector>

#include <cryptopp/secblock.h>
#include <cryptopp/gcm.h>
#include <cryptopp/osrng.h>

#include <aes.h>
using std::string;
using std::cout;
using std::endl;
using std::cerr;

using std::ifstream;
using std::vector;

using namespace CryptoPP;


int encrypt(string password, string input_path)
{
    vector<std::byte> fileBytes = getInputBytes(input_path);
    return 0;
}
