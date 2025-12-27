#ifndef DUPDETECT_SHAUTILS_HPP
#define DUPDETECT_SHAUTILS_HPP
#include <openssl/sha.h>

// Source -
// https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
// Posted by Adam Lamers, modified by community. See post 'Timeline' for change
// history Retrieved 2025-12-21, License - CC BY-SA 4.0
extern "C" {
void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH],
                        char          outputBuffer[65]);

void sha256_string(char const* string, char outputBuffer[65]);

int sha256_file(char const* path, char outputBuffer[65]);
}  // extern "C"

#endif  // DUPDETECT_SHAUTILS_HPP