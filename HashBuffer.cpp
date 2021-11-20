#include "HashBuffer.hpp"
//#include <base64.h>

using namespace std;

string HashBuffer::Hexify() const
{
    constexpr static char hexChars[] = "0123456789ABCDEF";
    const size_t resultStringLength  = sizeof(_hash) * 2;
    char result[resultStringLength + 1];

    // Convert each byte of _hash to hex-string.
    for (size_t i = 0; i < sizeof(_hash); ++i)
    {
        result[i * 2]     = hexChars[(_hash[i] & 0xF0) >> 4];
        result[i * 2 + 1] = hexChars[_hash[i] & 0xF];
    }

    // Null-terminate string and return.
    result[resultStringLength] = 0;
    return result;
}

string HashBuffer::Base64ize() const
{
    //string result(BASE64_ENCODE_OUT_SIZE(HASH_BYTES_SIZE), '0');

    //base64_encode(_hash, sizeof(_hash), &result[0]);

    return "";//result;
}
