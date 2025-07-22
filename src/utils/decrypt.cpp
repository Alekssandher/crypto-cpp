#include "decrypt.hpp"
#include "get_input_bytes.hpp"
#include "save_file.hpp"

#include <cryptopp/secblock.h>
#include <cryptopp/gcm.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pwdbased.h>

#include <bits/stdc++.h>

#include <aes.h>

using std::string;

using namespace CryptoPP;

string dec(const string &password, std::vector<std::byte> &data, AppConfig config);

int decrypt(const string &password, AppConfig config)
{

    if (config.verbose) {
        std::cout << "Starting decryption process...\n";
        std::cout << "Reading input file: " << config.input << "\n";
    }

    std::vector<std::byte> bytes = getInputBytes(config.input);

    try
    {
        if (config.verbose) {
            std::cout << "Deriving key from password...\n";
        }

        string result = dec(password, bytes, config);

        if (config.verbose) {
            std::cout << "Saving decrypted content to: " << config.output << "\n";
        }

        saveFile(result, config.output);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}

string dec(const string &password, std::vector<std::byte> &data, AppConfig config)
{
    if(data.size() < 32)
    {
        throw std::runtime_error("Corrupted or Invalid data provided.");
    }

    if (config.verbose) {
        std::cout << "Extracting salt, IV, and ciphertext...\n";
    }

    const byte* salt = reinterpret_cast<const byte*>(data.data());
    const byte* iv = reinterpret_cast<const byte*>(data.data() + 16);
    const byte* cipherText = reinterpret_cast<const byte *>(data.data() + 32);
    size_t cipherTextLen = data.size() - 32;

    if (config.verbose) {
        std::cout << "Deriving key with " << config.iterations << " iterations...\n";
    }

    SecByteBlock key(32);
    PKCS5_PBKDF2_HMAC<SHA256> pbkdf;

    pbkdf.DeriveKey(key, key.size(), 0,
        reinterpret_cast<const byte*>(password.data()), password.size(),
        salt, 16,
        config.iterations
    );

    if (config.verbose) {
        std::cout << "Key derived. Initializing AES-GCM decryption...\n";
    }

    GCM<AES>::Decryption dec;
    
    dec.SetKeyWithIV(key, key.size(), iv, AES::BLOCKSIZE);

    string recovered;
    try
    {
        StringSource ss(cipherText, cipherTextLen, true,
            new AuthenticatedDecryptionFilter(dec,
                new StringSink(recovered)
            )
        );
    }
    catch(const std::exception& e)
    {
        throw std::runtime_error(string("Fail while dec: ") + e.what());
    }
    
    if (config.verbose) {
        std::cout << "Data successfully decrypted.\n";
    }

    return recovered;
}