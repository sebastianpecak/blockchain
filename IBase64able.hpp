#ifndef IBASE64ABLE_HPP
#define IBASE64ABLE_HPP

#include <string>

/**
 * Interface for implementing method that returns data base64 codded.
 */
class IBase64able
{
public:
    virtual std::string Base64ize() const = 0;
};

#endif  // IBASE64ABLE_HPP
