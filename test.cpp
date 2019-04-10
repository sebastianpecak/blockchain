#include <bitset>
#include <iostream>

using namespace std;

static bitset<256> number{};

int ___main()
{
    number.set();
    number <<= 128;

    cout << number;

    return 0;
}