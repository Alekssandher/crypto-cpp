#include "encrypt.hpp"
#include "get_input_bytes.hpp"
#include "save_file.hpp"

#include <bits/stdc++.h>
#include <vector>

#include <cryptopp/secblock.h>
#include <cryptopp/gcm.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pwdbased.h>

#include <aes.h>
using std::string;
using std::cout;
using std::endl;
using std::cerr;

using std::ifstream;
using std::vector;

using namespace CryptoPP;

string enc(const string& password, vector<std::byte> &data, AppConfig config);

int encrypt(const string& password, AppConfig config)
{
    if (config.verbose) {
        std::cout << "Starting encryption process...\n";
        std::cout << "Reading input file: " << config.input << "\n";
    }

    vector<std::byte> bytes = getInputBytes(config.input);

    if (config.verbose) {
        std::cout << "Deriving encryption key from password...\n";
    }

    std::string result = enc(password, bytes, config);

    if (config.verbose) {
        std::cout << "Saving encrypted data to: " << config.output << "\n";
    }

    saveFile(result, config.output);

    if (config.verbose) {
        std::cout << "Encryption completed successfully.\n";
    }

    return 0;
}

string enc(const string& password, vector<std::byte> &data, AppConfig config)
{
    if (config.verbose) {
        std::cout << "Generating random IV (" << AES::BLOCKSIZE << " bytes)...\n";
    }

    // Generate IV
    AutoSeededRandomPool prng;
    byte iv[AES::BLOCKSIZE];
    prng.GenerateBlock(iv, sizeof(iv));

    if (config.verbose) {
        std::cout << "Generating random salt (16 bytes)...\n";
    }

    byte salt[16];
    prng.GenerateBlock(salt, sizeof(salt));

    if (config.verbose) {
        std::cout << "Deriving key using PBKDF2 ("<< config.iterations << " iterations)...\n";
    }
    // Derivate key
    SecByteBlock key(32);
    PKCS5_PBKDF2_HMAC<SHA256> pbkdf;
    

    pbkdf.DeriveKey(key, key.size(), 0,
        reinterpret_cast<const byte*>(password.data()), password.size(),
        salt, sizeof(salt), 
        config.iterations 
    );

    if (config.verbose) {
        std::cout << "Encrypting data using AES-GCM...\n";
    }
    
    GCM<AES>::Encryption enc;
    enc.SetKeyWithIV(key, key.size(), iv, sizeof(iv));

    string cipher;
    StringSource ss(reinterpret_cast<const byte*>(data.data()), data.size(), true,
        new AuthenticatedEncryptionFilter(enc, new StringSink(cipher)));
    
    if (config.verbose) {
        std::cout << "Combining salt, IV, and ciphertext into final output...\n";
    }

    // Appending for future dec
    string output;
    output.reserve(sizeof(salt) + sizeof(iv) + cipher.size());
    output.append(reinterpret_cast<char*>(salt), sizeof(salt));
    output.append(reinterpret_cast<char*>(iv), sizeof(iv));
    output.append(cipher);

    if (config.verbose) {
        std::cout << "Encrypted data ready.\n";
    }

    return output;
}