#include "matrix_layer.hpp"
#include "matrix_cell.hpp"

#include <cassert>
#include <memory>

namespace controller {

class matrix_cell_factory : public core::cell_factory
{
public:
    core::cell* create_cell(unsigned r, unsigned c, core::layer *l)
    {
        return new matrix_cell(r, c, l);
    }
public:
    virtual ~matrix_cell_factory()
    {}
};

matrix_layer::matrix_layer(unsigned id, unsigned width, unsigned height)
    : core::layer(id, width, height, new matrix_cell_factory)
{
}

matrix_layer::~matrix_layer()
{}

matrix_layer* matrix_layer::clone(core::cell_factory*)
{
    matrix_layer* l = new matrix_layer(id(), width(), height());
    for (unsigned i = 0; i < height(); ++i) {
        for (unsigned j = 0; j < width(); ++j) {
            l->set_cell_value(i, j, get_cell_value(i, j));
            l->set_cell_value_1(i, j, get_cell_value_1(i, j));
            std::shared_ptr<matrix_cell> c = std::dynamic_pointer_cast<matrix_cell>(l->get_cell(i, j));
            std::shared_ptr<matrix_cell> c_old = std::dynamic_pointer_cast<matrix_cell>(get_cell(i, j));
            assert(c != 0);
            assert(c_old != 0);
            c->set_source_position(c_old->source_row(), c_old->source_column());
            c->add_item(c_old->get_item());
        }
    }
    return l;
}

void matrix_layer::generate_submatrixes(const unsigned size)
{
    m_submatrixes.resize(height() - size + 1);
    for (unsigned i = 0; i < height() - size + 1; ++i) {
        m_submatrixes[i].resize(width() - size + 1, 0);
        for (unsigned j = 0; j < width() - size + 1; ++j) {
            m_submatrixes[i][j] = new core::submatrix(this, size, core::index(i, j));
        }
    }
}

matrix_layer::submatrixes& matrix_layer::get_submatrixes()
{
    return m_submatrixes;
}

}
