#ifndef STRINGBLOCK_HPP
#define STRINGBLOCK_HPP

#include "BaseBlock.hpp"
#include <vector>

class StringBlock : public BaseBlock
{
    std::string _data;

protected:
    virtual void _UpdateDataHash();

public:
    StringBlock() {}
    StringBlock(const StringBlock&) {}
    virtual ~StringBlock(){ }
    
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

using StringBlockChain = std::vector<StringBlock>;

std::ostream& operator<<(std::ostream& stream, const StringBlockChain& chain);
std::istream& operator>>(std::istream& stream, StringBlockChain& chain);

#endif  // STRINGBLOCK_HPP
