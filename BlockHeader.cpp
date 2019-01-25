#include "BlockHeader.hpp"
#include <sstream>
#include <vector>
#include <sha256.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <limits>
#include <algorithm>

#define SIGINIFICANT_ZEROS_NUMBER 32
#define THREADS_MULTIPLIER 2

using namespace std;

static atomic<bool> hashFound = false;

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

bool BlockHeader::_IsHashFound(const std::vector<uint8_t>& serializedHeader)
{
    HashBuffer result{};

    sha256_hash(&_ctx, serializedHeader.data(), serializedHeader.size());
    sha256_done(&_ctx, result.Buffer());

    const uint64_t mask = UINT64_MAX << SIGINIFICANT_ZEROS_NUMBER;
    const uint64_t front = *(uint64_t*)result.Buffer();

    return (front & mask) == front;
}

void BlockHeader::_FindHashThread(uint32_t nonceStart, uint32_t range)
{
    cout << "THREAD STARTED, ID: " << this_thread::get_id() << endl;

    const size_t previousSize = _previousBlockHash.Size();
    const size_t dataSize = _dataRootHash.Size();
    const size_t nonceSize = sizeof(nonceStart);
    const size_t nonceOffset = previousSize + dataSize;

    vector<uint8_t> serializedHeader(previousSize + dataSize + nonceSize);

    memcpy(serializedHeader.data(), _previousBlockHash.Buffer(), previousSize);
    memcpy(serializedHeader.data() + previousSize, _dataRootHash.Buffer(), dataSize);

    do
    {
        // Serialize nonce.
        memcpy(serializedHeader.data() + nonceOffset, &nonceStart, nonceSize);
        ++nonceStart;
        --range;
    } while (!_IsHashFound(serializedHeader) && range > 0 && !hashFound);

    if (_IsHashFound(serializedHeader))
    {
        hashFound = true;
        cout << "HASH FOUND FOR NONCE: " << nonceStart << ", THREAD ID: " << this_thread::get_id() << endl;
    }
    else
    {
        cout << "ENDING " << this_thread::get_id() << " THREAD" << endl;
    }
}

void BlockHeader::Validate()
{
    size_t tries = 0;
    _UpdateDataHash();

    uint32_t threadsNo = thread::hardware_concurrency() * THREADS_MULTIPLIER;
    uint32_t rangePerThread = numeric_limits<uint32_t>::max() / threadsNo;

    vector<thread> threads{};

    for (uint32_t threadID = 0; threadID < threadsNo; ++threadID)
    {
        threads.emplace_back(thread(&BlockHeader::_FindHashThread, this, rangePerThread * threadID, rangePerThread));
    }

    for_each(threads.begin(), threads.end(), [](thread& t) {t.join(); });

    //auto startTime = chrono::high_resolution_clock::now();

    //// Bruteforce.
    //while (!IsValid())
    //{
    //    ++_nonce;
    //    ++tries;
    //}

    //auto endTime = chrono::high_resolution_clock::now();

    //cout 
    //    << "BLOCK VALIDATED, TRIES COUNTER: " 
    //    << tries 
    //    << ", HASH: " 
    //    << GetHash().Hexify()
    //    << ", TIME TAKEN: "
    //    << chrono::duration_cast<chrono::seconds>(endTime - startTime).count()
    //    <<"s"
    //    << endl;
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

    sha256_context _ctx{};

    sha256_init(&_ctx);
    sha256_hash(&_ctx, serializedHeader.data(), serializedHeader.size());
    sha256_done(&_ctx, result.Buffer());

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
