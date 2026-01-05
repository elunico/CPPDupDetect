#include "shautils.hpp"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "openssl/sha.h"

extern "C" {

#include <openssl/evp.h>

void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH],
                        char          outputBuffer[65])
{
    int i = 0;

    for (i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        snprintf(outputBuffer + i * 2, 3, "%02x", hash[i]);
    }

    outputBuffer[64] = 0;
}

// adapted from: https://wiki.openssl.org/index.php/EVP_Message_Digests
// Returns 0 on success, non-zero on failure
inline int digest_file(const char* path, char output[65])
{
    EVP_MD_CTX* mdctx;
    FILE*       file = fopen(path, "rb");

    if (file == NULL) {
        return 1;  // Failed to open file
    }

    constexpr int buf_size   = 32768;
    auto*         buffer     = static_cast<unsigned char*>(malloc(buf_size));
    int           bytes_read = 0;

    if (buffer == NULL) {
        fclose(file);
        return 2;  // Failed to allocate buffer
    }

    unsigned char* digest;
    if ((digest = (unsigned char*) OPENSSL_malloc(EVP_MD_size(EVP_sha256()))) ==
        NULL) {
        free(buffer);
        fclose(file);
        return 3;  // Failed to allocate digest
    }

    if ((mdctx = EVP_MD_CTX_new()) == NULL) {
        OPENSSL_free(digest);
        free(buffer);
        fclose(file);
        return 4;  // Failed to create context
    }

    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL)) {
        EVP_MD_CTX_free(mdctx);
        OPENSSL_free(digest);
        free(buffer);
        fclose(file);
        return 5;  // Failed to initialize digest
    }

    while ((bytes_read = fread(buffer, 1, buf_size, file))) {
        if (1 != EVP_DigestUpdate(mdctx, buffer, bytes_read)) {
            EVP_MD_CTX_free(mdctx);
            OPENSSL_free(digest);
            free(buffer);
            fclose(file);
            return 6;  // Failed to update digest
        }
    }

    if (1 != EVP_DigestFinal_ex(mdctx, digest, NULL)) {
        EVP_MD_CTX_free(mdctx);
        OPENSSL_free(digest);
        free(buffer);
        fclose(file);
        return 7;  // Failed to finalize digest
    }

    sha256_hash_string(digest, output);

    EVP_MD_CTX_free(mdctx);
    OPENSSL_free(digest);
    free(buffer);
    fclose(file);
    return 0;  // Success
}

int sha256_file(char const* path, char outputBuffer[65])
{
    return digest_file(path, outputBuffer);
}

}  // extern "C"
