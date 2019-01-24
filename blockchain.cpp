#include <iostream>
#include "HashBuffer.hpp"
#include "BlockHeader.hpp"
#include "StringBlock.hpp"
#include <fstream>

using namespace std;

int main()
{
    StringBlock stringBlock{};

    stringBlock.SetData("First coin found by Sebastian Pecak.");
    stringBlock.Validate();

    cout << stringBlock.Jsonize() << endl;

    fstream("test.blockchain", ios::out | ios::binary) << stringBlock;

    StringBlock readBlock{};
    fstream("test.blockchain", ios::in | ios::binary) >> readBlock;

    cout << readBlock.Jsonize() << endl;

    return 0;
}