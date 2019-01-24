#ifndef IHEXIFIABLE_HPP
#define IHEXIFIABLE_HPP

#include <string>

class IHexifiable
{
public:
    virtual std::string Hexify() const = 0;
};

#endif  // IHEXIFIABLE_HPP
