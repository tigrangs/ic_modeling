#include "layer.hpp"
#include "cell.hpp"

#include <cassert>

namespace core
{

layer::layer(unsigned id, unsigned width, unsigned height)
    : m_id(id)
    , m_width(width)
    , m_height(height)
{
        init();
}

layer::~layer()
{
        for (unsigned i = 0; i < m_height; ++i) {
                for (unsigned j = 0; j < m_width; ++j) {
                        delete m_cells[i][j];
                }
        }
}

void layer::init()
{
        m_cells.reserve(m_height);
        for (unsigned i = 0; i < m_height; ++i) {
                m_cells[i].reserve(m_width);
                for (unsigned j = 0; j < m_width; ++j) {
                        m_cells[i][j] = new cell(i, j, this);
                }
        }
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

void layer::dump(std::string& s) const
{
    s += "\n";
    s += "******************* Layer " + std::to_string(m_id) + " **********************";
    for (unsigned i = 0; i < m_height; ++i) {
        for (unsigned j = 0; j < m_width; ++j) {
            cell* c = m_cells[i][j];
            assert(c != 0);
            c->dump(s);
        }
    }
}

double layer::get_cell_value(unsigned r, unsigned c)
{
    cell* tc = m_cells[r][c];
    assert(tc != 0);
    return tc->value();
}

void layer::set_cell_value(unsigned r, unsigned c, double v)
{
    cell* tc = m_cells[r][c];
    assert(tc != 0);
    tc->set_value(v);
}

}
