#include "BlockHeader.hpp"
#include <sstream>
#include <vector>
#include <sha256.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <limits>
#include <algorithm>
#include <tuple>
#include <iomanip>

#define SIGINIFICANT_ZEROS_NUMBER 25
#define THREADS_MULTIPLIER 2

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

bool BlockHeader::_IsHashFound(const std::vector<uint8_t>& serializedHeader)
//tuple<bool, HashBuffer> BlockHeader::_IsHashFound(const std::vector<uint8_t>& serializedHeader)
{
    HashBuffer result{};
    sha256_context ctx{};

    sha256_init(&ctx);
    sha256_hash(&ctx, serializedHeader.data(), serializedHeader.size());
    sha256_done(&ctx, result.Buffer());

    const uint64_t mask = UINT64_MAX << SIGINIFICANT_ZEROS_NUMBER;
    const uint64_t front = *(uint64_t*)result.Buffer();

    const bool found = (front & mask) == front;

    if (found)
    {
        _Log("Hash found: %s", result.Hexify().c_str());
    }

    return found;
    //return make_tuple(found, result);
}

ostream& operator<<(ostream& stream, const vector<uint8_t>& bytes)
{
    for (const auto& i : bytes)
    {
        stream /*<< "0x"*/ << setfill('0') << setw(2) << hex << (int)i /*<< " "*/;
    }

    stream << endl;

    return stream;
}

void BlockHeader::_FindHashThread(uint32_t nonceStart, uint32_t range)
{
    _Log("Thread started, nonceStart: %u, range: %u", nonceStart, range);

    const size_t previousSize = _previousBlockHash.Size();
    const size_t dataSize = _dataRootHash.Size();
    const size_t nonceSize = sizeof(nonceStart);
    const size_t nonceOffset = previousSize + dataSize;

    vector<uint8_t> serializedHeader(previousSize + dataSize + nonceSize);

    memcpy(serializedHeader.data(), _previousBlockHash.Buffer(), previousSize);
    memcpy(serializedHeader.data() + previousSize, _dataRootHash.Buffer(), dataSize);

    bool foundHash = false;
    for (;;)
    {
        // Serialize nonce.
        memcpy(serializedHeader.data() + nonceOffset, &nonceStart, nonceSize);

        foundHash = _IsHashFound(serializedHeader);

        if (foundHash)
        {
            // Notify threads to stop seeking.
            _hashFound.store(true);
            _nonce = nonceStart;
            break;
        }

        ++nonceStart;
        --range;

        if (range == 0 || _hashFound.load())
        {
            break;
        }
    }

    //bool found = false;
    ////tuple<bool, HashBuffer> res{};
    //do
    //{
    //    // Serialize nonce.
    //    memcpy(serializedHeader.data() + nonceOffset, &nonceStart, nonceSize);
    //    ++nonceStart;
    //    --range;
    //    res = _IsHashFound(serializedHeader);
    //} while (!get<0>(res)/*!_IsHashFound(serializedHeader)*/ && range > 0 && !_hashFound.load());

    //string aaa = get<1>(res).Hexify();

    //string bbb = get<1>(_IsHashFound(serializedHeader)).Hexify();

    /*if (foundHash)
    {
        _Log("Hash found for nonce: %u", nonceStart);
        _nonce = nonceStart;
    }*/
    /*else
    {
        _Log("Hash was not found");
    }*/
}

void BlockHeader::Validate()
{
    size_t tries = 0;
    _UpdateDataHash();

    const size_t threadsNo = thread::hardware_concurrency() * THREADS_MULTIPLIER;
    const size_t typeCapacity = numeric_limits<uint32_t>::max();
    const size_t rangePerThread = (typeCapacity + 1) / threadsNo;

    vector<thread> threads{};

    _hashFound.store(false);

    for (uint32_t threadID = 0; threadID < threadsNo; ++threadID)
    {
        threads.emplace_back(thread(&BlockHeader::_FindHashThread, this, rangePerThread * threadID, rangePerThread));
    }

    for_each(threads.begin(), threads.end(), [](thread& t) {t.join(); });

    cout << "THREADS JOINED" << endl;

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
