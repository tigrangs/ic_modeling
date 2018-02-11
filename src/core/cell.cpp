#include "cell.hpp"

namespace cell
{

cell::cell(unsigned r, unsigned c, unsigned l)
    : m_row(r)
    , m_column(c)
    , m_layer(l)
    , m_value(0)
{
}

void cell::value() const
{
    return m_value;
}

int cell::set_value(int v)
{
    m_value = v;
}

}
