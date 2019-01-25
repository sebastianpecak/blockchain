#ifndef BLOCKHEADER_HPP
#define BLOCKHEADER_HPP

#include "IJsonable.hpp"
#include "HashBuffer.hpp"
#include <iostream>
#include <vector>
#include <sha256.h>

class BlockHeader : public IJsonable
{
    HashBuffer _previousBlockHash{};
    HashBuffer _dataRootHash{};
    uint32_t _nonce = 0;

    sha256_context _ctx{};

protected:
    BlockHeader()
    {
        sha256_init(&_ctx);
    }

    virtual void _UpdateDataHash() {}

    void _FindHashThread(uint32_t nonceStart, uint32_t range);
    bool _IsHashFound(const std::vector<uint8_t>& serializedHeader);

public:
    virtual ~BlockHeader() {}

    virtual std::string Jsonize() const;

    bool IsValid() const;
    void Validate();

    inline HashBuffer& GetDataRootHash()
    {
        return _dataRootHash;
    }

    inline HashBuffer& GetPreviousBlockHash()
    {
        return _previousBlockHash;
    }

    // Calculate header hash.
    HashBuffer GetHash() const;

    // Header serializer.
    friend std::ostream& operator<<(std::ostream& stream, const BlockHeader& header);
    // Header deserializer.
    friend std::istream& operator>>(std::istream& stream, BlockHeader& header);
};

#endif  // BLOCKHEADER_HPP
