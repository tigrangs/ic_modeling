#include "layer.hpp"
#include "cell.hpp"

#include <cassert>
#include <iostream>

namespace core
{

layer::layer(unsigned id, unsigned width, unsigned height, cell_factory* f)
    : m_id(id)
    , m_width(width)
    , m_height(height)
{
    init(f ? f : new cell_factory);
    std::cout<<"INIT"<<std::endl;
}

layer::~layer()
{
/*    for (unsigned i = 0; i < m_height; ++i) {
        for (unsigned j = 0; j < m_width; ++j) {
            cell* c = m_cells.at(i).at(j);
            delete c;
            m_cells[i][j] = 0;
        }
    }*/
}

layer* layer::clone(cell_factory* f)
{
    layer* l = new layer(m_id, m_width, m_height, f);
    for (unsigned i = 0; i < m_height; ++i) {
        for (unsigned j = 0; j < m_width; ++j) {
            l->set_cell_value(i, j, get_cell_value(i, j));
            l->set_cell_value_1(i, j, get_cell_value_1(i, j));
        }
    }
    return l;
}

void layer::init(cell_factory* f)
{
    assert(f != 0);
    m_cells.reserve(m_height);
    for (unsigned i = 0; i < m_height; ++i) {
        m_cells.push_back(std::vector<std::shared_ptr<cell>>(m_width, 0));
        //m_cells.back().reserve(m_width);
        for (unsigned j = 0; j < m_width; ++j) {
            m_cells.back()[j] = std::shared_ptr<cell>(f->create_cell(i, j, this));
        }
    }
    assert(m_cells.size() == m_height);
    assert(m_cells[0].size() == m_width);
    delete f;
}

std::shared_ptr<cell> layer::get_cell(unsigned r, unsigned c)
{
    assert(r < m_height);
    assert(c <m_width);
    return m_cells[r][c];
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
            std::shared_ptr<cell> c = m_cells[i][j];
            assert(c != 0);
            c->dump(s);
            std::cout<<"Cell "<<i<<" "<<j<<std::endl;
        }
    }
}

double layer::get_cell_value(unsigned r, unsigned c)
{
    assert(r < m_height);
    assert(c < m_width);
    std::shared_ptr<cell> tc = m_cells[r][c];
    assert(tc != 0);
    return tc->value();
}

void layer::set_cell_value(unsigned r, unsigned c, double v)
{
    assert(r < m_height);
    assert(c < m_width);
    std::shared_ptr<cell> tc = m_cells[r][c];
    assert(tc != 0);
    tc->set_value(v);
}

double layer::get_cell_value_1(unsigned r, unsigned c)
{
    assert(r < m_height);
    assert(c < m_width);
    std::shared_ptr<cell> tc = m_cells[r][c];
    assert(tc != 0);
    return tc->value_1();
}

void layer::set_cell_value_1(unsigned r, unsigned c, double v)
{
    assert(r < m_height);
    assert(c < m_width);
    std::shared_ptr<cell> tc = m_cells[r][c];
    assert(tc != 0);
    tc->set_value_1(v);
}

}
