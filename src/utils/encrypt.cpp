#include "encrypt.hpp"
#include "get_input_bytes.hpp"
#include "save_file.hpp"

#include <vector>
#include <sodium.h>

using std::string;
using std::vector;

std::vector<std::byte> enc(const string& password, vector<std::byte> &data, AppConfig config);

int encrypt(const string& password, AppConfig config)
{
    vector<std::byte> bytes = getInputBytes(config.input);

    std::vector<std::byte> result = enc(password, bytes, config);

    saveFile(result, config.output);
    return 0;
}

std::vector<std::byte> enc(const string& password, vector<std::byte> &data, AppConfig config)
{
    const size_t SALT_LEN = crypto_pwhash_SALTBYTES;
    const size_t KEY_LEN = crypto_aead_chacha20poly1305_ietf_KEYBYTES;
    const size_t NONCE_LEN = crypto_aead_chacha20poly1305_ietf_NPUBBYTES;

    if (config.verbose) {
        std::cout << "Generating salt and nonce..." << std::endl;
    }

    unsigned char salt[SALT_LEN];
    unsigned char nonce[NONCE_LEN];
    randombytes_buf(salt, SALT_LEN);
    randombytes_buf(nonce, NONCE_LEN);

    unsigned char key[KEY_LEN];

    if (crypto_pwhash(key, KEY_LEN,
                      password.c_str(), password.length(),
                      salt,
                      config.getOpsLimit(),
                      config.getMemLimit(),
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        throw std::runtime_error("Key derivation failed (out of memory?).");
    }

    if (config.verbose) {
        std::cout << "Encrypting data using ChaCha20-Poly1305..." << std::endl;
    }

    size_t plain_len = data.size();
    size_t cipher_len = plain_len + crypto_aead_chacha20poly1305_ietf_ABYTES;

    vector<unsigned char> ciphertext(cipher_len);

    crypto_aead_chacha20poly1305_ietf_encrypt(
        ciphertext.data(),         
        nullptr,                  
        reinterpret_cast<unsigned char*>(data.data()), plain_len, 
        nullptr, 0,                
        nullptr,                   
        nonce,                     
        key                       
    );

    if (config.verbose) {
        std::cout << "Combining salt + nonce + ciphertext for output..." << std::endl;
    }

    std::vector<std::byte> output;
    output.reserve(SALT_LEN + NONCE_LEN + cipher_len);

    // Append salt
    output.insert(output.end(),
                reinterpret_cast<std::byte*>(salt),
                reinterpret_cast<std::byte*>(salt + SALT_LEN));

    // Append nonce
    output.insert(output.end(),
                reinterpret_cast<std::byte*>(nonce),
                reinterpret_cast<std::byte*>(nonce + NONCE_LEN));

    // Append ciphertext
    output.insert(output.end(),
                reinterpret_cast<std::byte*>(ciphertext.data()),
                reinterpret_cast<std::byte*>(ciphertext.data() + cipher_len));

    if (config.verbose) {
        std::cout << "Encryption complete. Output length: " << output.size() << " bytes\n";
    }

    return output;
}