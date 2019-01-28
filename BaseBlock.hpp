#ifndef BASEBLOCK_HPP
#define BASEBLOCK_HPP

#include "BlockHeader.hpp"

class BaseBlock : public BlockHeader
{
public:
    BaseBlock() = default;
    virtual ~BaseBlock() {}
};

#endif  // BASEBLOCK_HPP
