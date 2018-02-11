#include "layer.hpp"

namespace core
{

layer::layer(int id, unsigned width, unsigned height)
    : m_id(id)
    , m_width(width)
    , m_height(height)
{}

int layer::id() const
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
