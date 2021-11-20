#ifndef IJSONABLE_HPP
#define IJSONABLE_HPP

#include <string>

/**
 * Interface for implementing method that returns data codded in Json format.
 */
class IJsonable
{
public:
    virtual std::string Jsonize() const = 0;
};

#endif  // IJSONABLE_HPP
