#ifndef BLOCKHEADER_HPP
#define BLOCKHEADER_HPP

#include "IJsonable.hpp"
#include "HashBuffer.hpp"
#include <iostream>

class BlockHeader : public IJsonable
{
    HashBuffer _previousBlockHash;
    HashBuffer _dataRootHash;
    uint64_t _nonce;

protected:
    virtual void _UpdateDataHash() = 0;

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

    // Header serializer.
    friend std::ostream& operator<<(std::ostream& stream, const BlockHeader& header);
    // Header deserializer.
    friend std::istream& operator>>(std::istream& stream, BlockHeader& header);
};

#endif  // BLOCKHEADER_HPP
