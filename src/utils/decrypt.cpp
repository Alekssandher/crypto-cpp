#include "decrypt.hpp"
#include "get_input_bytes.hpp"
#include "save_file.hpp"

#include <sodium.h>

using std::string;

std::vector<std::byte> dec(const std::string& password, const std::vector<std::byte>& input, AppConfig config);

int decrypt(const std::string& password, AppConfig config)
{
    std::vector<std::byte> input = getInputBytes(config.input);
    std::vector<std::byte> plaintext = dec(password, input, config);
    saveFile(plaintext, config.output);
    return 0;
}


std::vector<std::byte> dec(const std::string& password, const std::vector<std::byte>& input, AppConfig config)
{
    const size_t SALT_LEN = crypto_pwhash_SALTBYTES;
    const size_t NONCE_LEN = crypto_aead_chacha20poly1305_ietf_NPUBBYTES;
    const size_t KEY_LEN = crypto_aead_chacha20poly1305_ietf_KEYBYTES;

    if (input.size() < SALT_LEN + NONCE_LEN + crypto_aead_chacha20poly1305_ietf_ABYTES) {
        throw std::runtime_error("Input data is too short or corrupted.");
    }

    if (config.verbose) {
        std::cout << "Extracting salt, nonce and ciphertext..." << std::endl;
    }

    const unsigned char* salt = reinterpret_cast<const unsigned char*>(input.data());
    const unsigned char* nonce = reinterpret_cast<const unsigned char*>(input.data() + SALT_LEN);
    const unsigned char* ciphertext = reinterpret_cast<const unsigned char*>(input.data() + SALT_LEN + NONCE_LEN);
    const size_t ciphertext_len = input.size() - SALT_LEN - NONCE_LEN;

    unsigned char key[KEY_LEN];
    if (crypto_pwhash(key, KEY_LEN,
                      password.c_str(), password.length(),
                      salt,
                      config.getOpsLimit(),
                      config.getMemLimit(),
                      crypto_pwhash_ALG_DEFAULT) != 0) {
        throw std::runtime_error("Key derivation failed.");
    }

    std::vector<std::byte> decrypted(ciphertext_len); 
    unsigned long long decrypted_len;

    if (config.verbose) {
        std::cout << "Decrypting data..." << std::endl;
    }

    if (crypto_aead_chacha20poly1305_ietf_decrypt(
            reinterpret_cast<unsigned char*>(decrypted.data()), &decrypted_len,
            nullptr, 
            ciphertext, ciphertext_len,
            nullptr, 0,
            nonce,
            key) != 0) {
        throw std::runtime_error("Decryption failed. Possibly wrong password or corrupted file.");
    }

    decrypted.resize(decrypted_len); 
    return decrypted;
}