# crypto-cpp

A secure file encryption and decryption tool built with C++ and libsodium, developed as a CS50 final project.

## Showcase
[![Watch the video](https://img.youtube.com/vi/QxDbdNdGEbc/hqdefault.jpg)](https://youtu.be/QxDbdNdGEbc)

## Overview

crypto-cpp is a command-line application that provides secure file encryption and decryption using modern cryptographic algorithms. It uses ChaCha20-Poly1305 for authenticated encryption and Argon2 for key derivation, ensuring both confidentiality and integrity of your files.

## Features

- **Strong Encryption**: Uses ChaCha20-Poly1305 AEAD (Authenticated Encryption with Associated Data)
- **Secure Key Derivation**: Implements Argon2 password hashing with configurable security levels
- **Cross-Platform**: Supports both Linux and Windows (via MinGW cross-compilation)
- **Secure File Deletion**: Optional secure overwriting of original files
- **User-Friendly CLI**: Clean command-line interface with comprehensive options
- **Password Protection**: Hidden password input for security

## Security Features

### Encryption Algorithm
- **ChaCha20-Poly1305**: Modern authenticated encryption algorithm
- **256-bit keys**: Strong cryptographic security
- **Random nonces**: Each encryption uses a unique nonce
- **Authentication**: Built-in integrity verification

### Key Derivation
- **Argon2**: Memory-hard password hashing function
- **Configurable security levels**:
  - Level 1 (Interactive): Fast for frequent use
  - Level 2 (Moderate): Balanced security/performance (default)
  - Level 3 (Sensitive): Maximum security for sensitive data

### Secure Deletion
- **Multiple overwrites**: Configurable number of random overwrites (1-100 times)
- **Final zero pass**: Ensures data is thoroughly destroyed
- **File system removal**: Complete file deletion after overwriting

## Installation

### Prerequisites

#### Linux
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake libsodium-dev pkg-config

# Fedora/RHEL
sudo dnf install gcc-c++ cmake libsodium-devel pkgconfig
```

#### Windows Cross-Compilation (from Linux)
```bash
# Ubuntu/Debian
sudo apt install mingw-w64 mingw-w64-tools

# You'll also need libsodium compiled for MinGW-w64
```

### Building

#### Linux (Dynamic Linking)
```bash
git clone https://github.com/Alekssandher/crypto-cpp/
cd crypto-cpp
git submodule update --init --recursive
./scripts/build-linux.sh
```

#### Linux (Static Linking)
```bash
./scripts/build-linux-static.sh
```

#### Windows (Cross-Compilation)
```bash
./scripts/build-win.sh
```

## Usage

### Basic Syntax
```bash
crypto-cpp -i <input_file> -o <output_file> --op <operation> [options]
```

### Commands

#### Encrypt a file
```bash
# Basic encryption
crypto-cpp -i document.txt -o document.txt.enc --op encrypt

# With high security level and verbose output
crypto-cpp -i sensitive.pdf -o sensitive.pdf.enc --op encrypt --sl 3 -v

# Encrypt and securely delete original
crypto-cpp -i secret.doc -o secret.doc.enc --op encrypt --dof --dof-times 7
```

#### Decrypt a file
```bash
# Basic decryption
crypto-cpp -i document.txt.enc -o document.txt --op decrypt

# With verbose output
crypto-cpp -i document.txt.enc -o document.txt --op decrypt -v
```

### Options

| Option | Long Form | Description |
|--------|-----------|-------------|
| `-i` | `--input` | Input file path (required) |
| `-o` | `--output` | Output file path (required) |
| `--op` | `--operation` | Operation: `encrypt`/`enc` or `decrypt`/`dec` (required) |
| `--sl` | `--security-level` | Security level: 1 (fast), 2 (moderate), 3 (sensitive) |
| `-v` | `--verbose` | Show detailed operation information |
| `--dof` | `--deleteOriginalFile` | Securely delete the original file after operation |
| `--doft` | `--dof-times` | Number of overwrite passes for secure deletion (1-100) |

### Security Levels

| Level | Performance | Memory Usage | Use Case |
|-------|-------------|--------------|----------|
| 1 (Interactive) | Fast | Low | Frequent encryption/decryption |
| 2 (Moderate) | Balanced | Medium | General purpose (default) |
| 3 (Sensitive) | Slow | High | Maximum security for sensitive data |

## Examples

### Encrypting a Document
```bash
# Encrypt a document with moderate security
crypto-cpp -i important_document.pdf -o important_document.pdf.enc --op encrypt

# The program will prompt for password (hidden input):
# Type a password: [hidden]
# Retype password: [hidden]
```

### Decrypting a Document
```bash
# Decrypt the document
crypto-cpp -i important_document.pdf.enc -o recovered_document.pdf --op decrypt

# The program will prompt for password:
# Type a password: [hidden]
```

### High-Security Encryption with Secure Deletion
```bash
# Maximum security with 10-pass secure deletion
crypto-cpp -i top_secret.txt -o top_secret.txt.enc --op encrypt --sl 3 --dof --dof-times 10 -v
```

## File Format

Encrypted files use the following binary format:
```
[32-byte salt][12-byte nonce][encrypted data + 16-byte auth tag]
```

- **Salt**: Random salt for key derivation
- **Nonce**: Random nonce for ChaCha20-Poly1305
- **Encrypted Data**: The actual encrypted file content
- **Auth Tag**: Poly1305 authentication tag for integrity verification

## Security Considerations

### Best Practices
- Use strong, unique passwords
- Consider using security level 3 for sensitive data
- Enable secure deletion (`--dof`) when encrypting sensitive files
- Store encrypted files and passwords separately
- Verify file integrity after encryption/decryption

### Limitations
- Password strength depends on user choice
- Secure deletion effectiveness varies by storage medium (SSDs vs HDDs)
- Cross-platform compatibility requires proper libsodium installation

## Building from Source

### Project Structure
```
crypto-cpp/
├── CMakeLists.txt              # Build configuration
├── include/                    # Header files
├── src/                        # Source files
│   ├── main.cpp               # Entry point
│   ├── app_config.cpp         # Configuration handling
│   ├── handlers/              # Command handlers
│   └── utils/                 # Utility functions
├── scripts/                   # Build scripts
├── external/                  # External dependencies
└── mingw-w64.toolchain.cmake  # Windows cross-compilation
```

### Dependencies
- **libsodium**: Cryptographic library
- **CLI11**: Command-line parsing (included as submodule)
- **C++17**: Modern C++ standard

## Contributing

This project was developed as a CS50 final project. While primarily educational, contributions and improvements are welcome!

### Development Setup
1. Clone the repository with submodules
2. Install development dependencies
3. Use the provided build scripts
4. Follow C++ best practices and maintain code style

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- **CS50**: Harvard's Introduction to Computer Science course
- **libsodium**: The cryptographic library that makes this project possible
- **CLI11**: Modern command-line parsing for C++

## Disclaimer

This software is provided as-is for educational purposes. While it implements industry-standard cryptographic algorithms, users should evaluate its suitability for their specific security requirements. Always backup important data before encryption.

