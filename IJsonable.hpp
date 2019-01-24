#ifndef IJSONABLE_HPP
#define IJSONABLE_HPP

#include <string>

class IJsonable
{
public:
    virtual std::string Jsonize() const = 0;
};

#endif  // IJSONABLE_HPP
