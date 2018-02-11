#include "layer.hpp"
#include "cell.hpp"

namespace core
{

layer::layer(unsigned id, unsigned width, unsigned height)
    : m_id(id)
    , m_width(width)
    , m_height(height)
{

}

unsigned layer::id() const
{
    return m_id;
}

unsigned layer::width() const
{
    return m_width;
}

unsigned layer::height() const
{
    return m_height;
}

}
