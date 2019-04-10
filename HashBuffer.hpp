#ifndef HASHBUFFER_HPP
#define HASHBUFFER_HPP

#include "IHexifiable.hpp"
#include "IBase64able.hpp"
#include <cstdint>

#define HASH_BYTES_SIZE 32
#define HASH_BITS 256

class HashBuffer : public IHexifiable, public IBase64able
{
    uint8_t _hash[HASH_BYTES_SIZE]{};

public:
    HashBuffer() = default;
    HashBuffer(const HashBuffer&) = default;
    virtual ~HashBuffer() {}
    
    virtual std::string Hexify() const;
    virtual std::string Base64ize() const;

    inline uint8_t* Buffer()
    {
        return _hash;
    }

    inline const uint8_t* Buffer() const
    {
        return _hash;
    }

    // Returns hash bytes-span.
    inline size_t Size() const
    {
        return sizeof(_hash);
    }
};

#endif  // HASHBUFFER_HPP
