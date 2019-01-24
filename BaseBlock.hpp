#ifndef BASEBLOCK_HPP
#define BASEBLOCK_HPP

#include "BlockHeader.hpp"

class BaseBlock : public BlockHeader
{
public:
    virtual ~BaseBlock() {}
};

#endif  // BASEBLOCK_HPP
