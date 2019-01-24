#include "BlockHeader.hpp"
#include <sstream>
#include <vector>

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

bool BlockHeader::IsValid() const
{
    return false;
}

void BlockHeader::Validate()
{
    _UpdateDataHash();
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
