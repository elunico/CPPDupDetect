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
inline void digest_file(const char* path, char output[65])
{
    EVP_MD_CTX* mdctx;
    FILE*       file = fopen(path, "rb");

    if (file == NULL) {
        return;  // nothing to clean up
    }

    constexpr int  buf_size   = 32768;
    unsigned char* buffer     = static_cast<unsigned char*>(malloc(buf_size));
    int            bytes_read = 0;

    if (buffer == NULL) {
        fclose(file);
        return;
        // cannot jump over variable initiailizations
    }

    unsigned char* digest;
    if ((digest = (unsigned char*) OPENSSL_malloc(EVP_MD_size(EVP_sha256()))) ==
        NULL) {
        goto init_error;  // close file & free buffer
    }

    if ((mdctx = EVP_MD_CTX_new()) == NULL)
        goto error;  // close file, free buffer, free digest

    if (1 != EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL))
        goto up_error;

    while ((bytes_read = fread(buffer, 1, buf_size, file))) {
        if (1 != EVP_DigestUpdate(mdctx, buffer, bytes_read)) {
            goto up_error;
        }
    }

    if (1 != EVP_DigestFinal_ex(mdctx, digest, NULL))
        goto up_error;

    sha256_hash_string(digest, output);

up_error:
    EVP_MD_CTX_free(mdctx);
error:
    OPENSSL_free(digest);
init_error:
    free(buffer);
    fclose(file);
}

int sha256_file(char const* path, char outputBuffer[65])
{
    digest_file(path, outputBuffer);
    return 0;
}

}  // extern "C"
