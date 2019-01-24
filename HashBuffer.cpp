#include "HashBuffer.hpp"
#include <base64.h>

using namespace std;

string HashBuffer::Hexify() const
{
    static const char hexChars[] = "0123456789ABCDEF";

    string result(sizeof(_hash) * 2, '0');

    for (size_t i = 0; i < sizeof(_hash); ++i)
    {
        result[i * 2] = hexChars[(_hash[i] & 0xF0) >> 4];
        result[i * 2 + 1] = hexChars[_hash[i] & 0xF];
    }

    return result;
}

string HashBuffer::Base64ize() const
{
    string result(BASE64_ENCODE_OUT_SIZE(HASH_BYTES_SIZE), '0');

    base64_encode(_hash, sizeof(_hash), &result[0]);

    return result;
}
