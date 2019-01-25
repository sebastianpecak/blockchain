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

int main()
{
    //StringBlock stringBlock{};

    //stringBlock.SetData("Litwo! Ojczyzno moja! ty jesteś jak zdrowie: Ile cię trzeba cenić, ten tylko się dowie, Kto cię stracił. Dziś piękność twą w całej ozdobie Widzę i opisuję, bo tęsknię po tobie.");
    //stringBlock.Validate();

    ////bool isValid = stringBlock.IsValid();

    //cout << stringBlock.Jsonize() << endl;

    //fstream("test.blockchain", ios::out | ios::binary) << stringBlock;

    //StringBlock readBlock{};
    //fstream("test.blockchain", ios::in | ios::binary) >> readBlock;

    //cout << readBlock.Jsonize() << endl;

    _GenerateRandomBlockchain(10);

    return 0;
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
    vector<StringBlock> blockChain{};

    for (size_t i = 0; i < chainLength; ++i)
    {
        StringBlock block{};

        if (i > 0)
        {
            // Set previous block hash.
            block.GetPreviousBlockHash() = blockChain[i - 1].GetHash();
        }

        block.SetData(_GenerateRandomString(200));

        block.Validate();

        cout
            << "|--" << block.Jsonize() << " | BLOCK HASH: " << block.GetHash().Hexify() << endl
            << "|" << endl;

        blockChain.emplace_back(block);
    }

    // Save whole block chain.
    fstream file("test.blockchain", ios::out | ios::binary);

    for (const auto& i : blockChain)
    {
        file << i;
    }
}
