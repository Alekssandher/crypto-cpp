#include "encrypt.hpp"
#include "get_input_bytes.hpp"

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
    vector<std::byte> bytes = getInputBytes(config.input);

    enc(
        password,
        bytes,
        config
    );
    return 0;
}

string enc(const string& password, vector<std::byte> &data, AppConfig config)
{
    // Generate IV
    AutoSeededRandomPool prng;
    byte iv[AES::BLOCKSIZE];
    prng.GenerateBlock(iv, sizeof(iv));

    // Derivate key
    SecByteBlock key(32);
    PKCS5_PBKDF2_HMAC<SHA256> pbkdf;
    byte salt[16];
    prng.GenerateBlock(salt, sizeof(salt));

    pbkdf.DeriveKey(key, key.size(), 0,
        reinterpret_cast<const byte*>(password.data()), password.size(),
        salt, sizeof(salt), 
        config.iterations 
        );
    
    GCM<AES>::Encryption enc;
    enc.SetKeyWithIV(key, key.size(), iv, sizeof(iv));

    string cipher;
    StringSource ss(reinterpret_cast<const byte*>(data.data()), data.size(), true,
        new AuthenticatedEncryptionFilter(enc, new StringSink(cipher)));
    
    // Appending for future dec
    string output;
    output.append(reinterpret_cast<char*>(salt), sizeof(salt));
    output.append(reinterpret_cast<char*>(iv), sizeof(iv));
    output.append(cipher);

    return cipher;
}