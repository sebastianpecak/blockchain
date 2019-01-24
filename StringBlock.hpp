#ifndef STRINGBLOCK_HPP
#define STRINGBLOCK_HPP

#include "BaseBlock.hpp"

class StringBlock : public BaseBlock
{
    std::string _data;

protected:
    virtual void _UpdateDataHash();

public:
    void inline SetData(const std::string& data)
    {
        _data = data;
    }

    virtual std::string Jsonize() const;

    // Block serializer.
    friend std::ostream& operator<<(std::ostream& stream, const StringBlock& block);
    // Block deserializer.
    friend std::istream& operator>>(std::istream& stream, StringBlock& block);
};

#endif  // STRINGBLOCK_HPP
