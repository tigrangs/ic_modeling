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

public:
    layer(int id, unsigned width, unsigned height);

public:
    int id() const;
    unsigned width() const;
    unsigned height() const;

private:
    int m_id;
    unsigned m_width;
    unsigned m_height;

private:
    cells m_cells;
};

}

#endif // LAYER_HPP
