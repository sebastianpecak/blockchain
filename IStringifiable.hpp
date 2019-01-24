#ifndef ISTRINGIFIABLE_HPP
#define ISTRINGIFIABLE_HPP

#include <string>

class IStringifiable
{
public:
    virtual std::string Stringify() const = 0;
};

#endif  // ISTRINGIFIABLE_HPP
