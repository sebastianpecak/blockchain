#include <iostream>
#include "HashBuffer.hpp"
#include "BlockHeader.hpp"
#include "StringBlock.hpp"
#include <fstream>
#include <vector>
#include <ctime>
#include <future>

using namespace std;

static void _GenerateRandomBlockchain(size_t chainLength);
static string _GenerateRandomString(size_t maxLen);
static StringBlockChain _ReadBlockchain(const string& filename);

int main()
{
    _GenerateRandomBlockchain(10);

    _ReadBlockchain("test.blockchain");

    return 0;
}

vector<StringBlock> _ReadBlockchain(const string& filename)
{
    fstream f(filename.c_str(), ios::in | ios::binary);

    StringBlockChain output{};

    f >> output;

    return output;
}

string _GenerateRandomString(size_t maxLen)
{
    srand(time(nullptr));

    const size_t len = rand() % maxLen;

    string output(len, '*');

    for (auto& c : output)
    {
        c = 0x20 + (rand() % 94);
    }

    return output;
}

void _GenerateRandomBlockchain(size_t chainLength)
{
    StringBlockChain blockChain(chainLength);

    for (size_t i = 0; i < chainLength; ++i)
    {
        if (i > 0)
        {
            // Set previous block hash.
            blockChain[i].GetPreviousBlockHash() = blockChain[i - 1].GetHash();
        }

        blockChain[i].SetData(_GenerateRandomString(200));
        blockChain[i].Validate();
    }

    // Save whole block chain.
    fstream file("test.blockchain", ios::out | ios::binary);

    file << blockChain;
}
