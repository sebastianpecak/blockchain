#ifndef IBASE64ABLE_HPP
#define IBASE64ABLE_HPP

#include <string>

class IBase64able
{
public:
    virtual std::string Base64ize() const = 0;
};

#endif  // IBASE64ABLE_HPP
