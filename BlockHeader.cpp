#include "BlockHeader.hpp"
#include <sstream>
#include <vector>
#include <sha256.h>
#include <chrono>

#define SIGINIFICANT_ZEROS_NUMBER 20

using namespace std;

string BlockHeader::Jsonize() const
{
    stringstream builder{};

    builder
        << "{\"previous_block_hash\":\""
        << _previousBlockHash.Hexify()
        << "\",\"data_root_hash\":\""
        << _dataRootHash.Hexify()
        << "\",\"nonce\":"
        << _nonce
        << "}";

    return builder.str();
}

// Checks for partial colisions.
bool BlockHeader::IsValid() const
{
    const size_t maxZeros = HASH_BYTES_SIZE * 8;

    HashBuffer headerHash = GetHash();

    //cout << headerHash.Hexify() << endl;

    // For little-endian shift rigth.
    const uint64_t mask = UINT64_MAX << SIGINIFICANT_ZEROS_NUMBER;

    /*for (uint8_t* i = (uint8_t*)&mask; i < (uint8_t*)&mask + 8; ++i)
    {
        printf("%u ", *i);
    }*/

    //puts("");

    //uint64_t front = 0;
    //memcpy(&front, headerHash.Buffer(), sizeof(front));
    const uint64_t front = *(uint64_t*)headerHash.Buffer();

    /*for (uint8_t* i = (uint8_t*)&front; i < (uint8_t*)&front + 8; ++i)
    {
        printf("%u ", *i);
    }*/

    return (front & mask) == front;
}

void BlockHeader::Validate()
{
    size_t tries = 0;
    _UpdateDataHash();

    auto startTime = chrono::high_resolution_clock::now();

    // Bruteforce.
    while (!IsValid())
    {
        ++_nonce;
        ++tries;
    }

    auto endTime = chrono::high_resolution_clock::now();

    /*cout 
        << "BLOCK VALIDATED, TRIES COUNTER: " 
        << tries 
        << ", HASH: " 
        << GetHash().Hexify()
        << ", TIME TAKEN: "
        << chrono::duration_cast<chrono::seconds>(endTime - startTime).count()
        <<"s"
        << endl;*/
}

HashBuffer BlockHeader::GetHash() const
{
    HashBuffer result{};

    const size_t previousSize = _previousBlockHash.Size();
    const size_t dataSize = _dataRootHash.Size();
    const size_t nonceSize = sizeof(_nonce);

    vector<uint8_t> serializedHeader(previousSize + dataSize + nonceSize);

    memcpy(serializedHeader.data(), _previousBlockHash.Buffer(), previousSize);
    memcpy(serializedHeader.data() + previousSize, _dataRootHash.Buffer(), dataSize);
    memcpy(serializedHeader.data() + previousSize + dataSize, &_nonce, nonceSize);

    sha256_context ctx{};

    sha256_init(&ctx);
    sha256_hash(&ctx, serializedHeader.data(), serializedHeader.size());
    sha256_done(&ctx, result.Buffer());

    return result;
}

ostream& operator<<(ostream& stream, const BlockHeader& header)
{
    stream.write((const char*)header._previousBlockHash.Buffer(), header._previousBlockHash.Size());
    stream.write((const char*)header._dataRootHash.Buffer(), header._dataRootHash.Size());
    stream.write((char*)&header._nonce, sizeof(header._nonce));

    return stream;
}

istream& operator>>(istream& stream, BlockHeader& header)
{
    stream.read((char*)header._previousBlockHash.Buffer(), header._previousBlockHash.Size());
    stream.read((char*)header._dataRootHash.Buffer(), header._dataRootHash.Size());
    stream.read((char*)&header._nonce, sizeof(header._nonce));

    return stream;
}
