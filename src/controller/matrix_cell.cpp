#include "matrix_cell.hpp"

#include <cassert>

namespace controller {

matrix_cell::matrix_cell(unsigned r, unsigned c, const core::layer* l)
    : core::cell(r, c, l)
{
}

void matrix_cell::set_source_position(unsigned r, unsigned c)
{
    m_source_row = r;
    m_source_column = c;
}

void matrix_cell::add_item(QGraphicsRectItem *item)
{
    assert(item != 0);
    m_item = item;
}

}
