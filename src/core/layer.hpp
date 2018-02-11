#ifndef LAYER_HPP
#define LAYER_HPP

#include <vector>

namespace core
{

class cell;

class layer
{
public:
    using cells =  std::vector<std::vector<cell*>>;

private:
    cells m_cells;
};

}

#endif // LAYER_HPP
