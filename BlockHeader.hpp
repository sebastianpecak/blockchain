#ifndef BLOCKHEADER_HPP
#define BLOCKHEADER_HPP

#include "IJsonable.hpp"
#include "HashBuffer.hpp"
#include <iostream>
#include <vector>
#include <sha256.h>
#include <atomic>
#include <mutex>
#include <thread>
#include <tuple>

class BlockHeader : public IJsonable
{
    HashBuffer _previousBlockHash{};
    HashBuffer _dataRootHash{};
    uint32_t _nonce = 0;
    std::atomic_bool _hashFound = false;
    std::mutex _logMutex{};
    std::vector<std::thread::id> ids{};

protected:
    virtual void _UpdateDataHash() {}

    void _FindHashThread(uint32_t nonceStart, uint32_t range);
    bool/*std::tuple<bool, HashBuffer>*/ _IsHashFound(const std::vector<uint8_t>& serializedHeader);
    
    template<typename... T>
    void _Log(const std::string& format, T&&... args)
    {
        std::lock_guard<std::mutex> lock(_logMutex);

        size_t i = 0;
        bool found = false;
        for (; i < ids.size(); ++i)
        {
            if (ids[i] == this_thread::get_id())
            {
                break;
            }
        }

        if (!found)
        {
            ids.push_back(this_thread::get_id());
            ++i;
        }

        printf(("%*.c[%.08u]: " + format + "\n").c_str(), i, ' ', this_thread::get_id(), std::forward<T&&>(args)...);
    }

public:
    BlockHeader() = default;
    BlockHeader(const BlockHeader&) = default;
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
