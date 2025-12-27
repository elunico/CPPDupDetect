#include "shautils.hpp"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {

void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH],
                        char          outputBuffer[65])
{
    int i = 0;

    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        snprintf(outputBuffer + i * 2, 3, "%02x", hash[i]);
    }

    outputBuffer[64] = 0;
}

void sha256_string(char const* string, char outputBuffer[65])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX    sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        snprintf(outputBuffer + i * 2, 3, "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

int sha256_file(char const* path, char outputBuffer[65])
{
    FILE* file = fopen(path, "rb");
    if (!file)
        return -534;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX    sha256;
    SHA256_Init(&sha256);
    constexpr int  bufSize   = 32768;
    unsigned char* buffer    = static_cast<unsigned char*>(malloc(bufSize));
    int            bytesRead = 0;
    if (!buffer)
        return ENOMEM;
    while ((bytesRead = fread(buffer, 1, bufSize, file))) {
        SHA256_Update(&sha256, buffer, bytesRead);
    }
    SHA256_Final(hash, &sha256);

    sha256_hash_string(hash, outputBuffer);
    fclose(file);
    free(buffer);
    return 0;
}

}  // extern "C"