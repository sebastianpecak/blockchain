#include "StringBlock.hpp"
#include <sstream>
#include <sha256.h>

using namespace std;

void StringBlock::_UpdateDataHash()
{
    sha256_context _ctx{};

    sha256_init(&_ctx);
    sha256_hash(&_ctx, (uint8_t*)_data.data(), _data.length());
    sha256_done(&_ctx, GetDataRootHash().Buffer());
}

string StringBlock::Jsonize() const
{
    stringstream builder{};

    builder
        << "{\"header\":"
        << BlockHeader::Jsonize()
        << ",\"string_data\":\""
        << _data
        << "\"}";

    return builder.str();
}

std::ostream& operator<<(std::ostream& stream, const StringBlock& block)
{
    stream << (BlockHeader&)block;

    const size_t dataLength = block._data.length();
    stream.write((char*)&dataLength, sizeof(dataLength));
    stream.write(block._data.data(), dataLength);

    return stream;
}

std::istream& operator>>(std::istream& stream, StringBlock& block)
{
    stream >> (BlockHeader&)block;

    size_t dataLength = 0;
    stream.read((char*)&dataLength, sizeof(dataLength));
    block._data.resize(dataLength);
    stream.read(&block._data[0], dataLength);

    return stream;
}
