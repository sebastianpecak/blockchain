#ifndef ISTRINGIFIABLE_HPP
#define ISTRINGIFIABLE_HPP

#include <string>

/**
 * Interface for implementing method that returns stringified data.
 */
class IStringifiable
{
public:
    virtual std::string Stringify() const = 0;
};

#endif  // ISTRINGIFIABLE_HPP
