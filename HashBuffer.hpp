#ifndef HASHBUFFER_HPP
#define HASHBUFFER_HPP

#include "IHexifiable.hpp"
#include "IBase64able.hpp"
#include <cstdint>

/**
 * Class that stores and operates hash.
 */
class HashBuffer : public IHexifiable, public IBase64able
{
    /**
     * Size of hash buffer in bytes.
     */
    constexpr static size_t HASH_SIZE_IN_BYTES = 32;
    /**
     * Raw hash buffer.
     */
    uint8_t _hash[HASH_SIZE_IN_BYTES];

public:
    /**
     * Returns hash converted to hex and base64.
     */
    virtual std::string Hexify() const;
    virtual std::string Base64ize() const;

    // inline const uint8_t* Buffer() const
    // {
    //     return _hash;
    // }

    // // Returns hash bytes-span.
    // inline size_t Size() const
    // {
    //     return sizeof(_hash);
    // }
};

#endif  // HASHBUFFER_HPP
