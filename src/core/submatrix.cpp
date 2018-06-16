#include "submatrix.hpp"

#include "layer.hpp"
#include <cassert>

namespace core {

submatrix::submatrix(layer* layer, const unsigned size, index ind)
    : m_layer(layer)
    , m_size(size)
    , m_index(ind)
    , m_sum_is_valid(false)
    , m_sum(0)
{
    assert(m_layer != 0);
    for(unsigned i = 0; i < m_size; ++i) {
        for(unsigned j = 0; j< m_size; ++j) {
            cell* c = m_layer->get_cell(m_index.m_row + i, m_index.m_column + j);
            c->add_submatrix(this);
            m_cells.push_back(c);
        }
    }
}

cell* submatrix::get_max_cell()
{
    unsigned max = m_cells[0]->value();
    cell* max_cell = m_cells[0];
    for (auto i: m_cells) {
        if (i->value() > max) {
            max = i->value();
            max_cell = i;
        }
    }
    return max_cell;
}

cell* submatrix::get_min_cell()
{
    unsigned min = m_cells[0]->value();
    cell* min_cell = m_cells[0];
    for (auto i: m_cells) {
        if (i->value() < min) {
            min = i->value();
            min_cell = i;
        }
    }
    return min_cell;
}

unsigned submatrix::get_sum() const
{
    if (!m_sum_is_valid) {
        recalculate_sum();
    }
    return m_sum;
}

void submatrix::recalculate_sum() const
{
    m_sum = 0;
    for (auto i: m_cells) {
        assert(i != 0);
        m_sum += i->value();
    }
    m_sum_is_valid = true;
}

void submatrix::invalidate()
{
    m_sum_is_valid = false;
}

submatrix::cells& submatrix::get_cells()
{
    return m_cells;
}

}
