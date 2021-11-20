#ifndef IHEXIFIABLE_HPP
#define IHEXIFIABLE_HPP

#include <string>

/**
 * Interface for implementing method that returns data codded hexadecimal.
 */
class IHexifiable
{
public:
    virtual std::string Hexify() const = 0;
};

#endif  // IHEXIFIABLE_HPP
