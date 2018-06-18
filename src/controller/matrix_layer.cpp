#include "matrix_layer.hpp"
#include "matrix_cell.hpp"

namespace controller {

class matrix_cell_factory : public core::cell_factory
{
public:
    core::cell* create_cell(unsigned r, unsigned c, core::layer *l)
    {
        return new matrix_cell(r, c, l);
    }

};

matrix_layer::matrix_layer(unsigned id, unsigned width, unsigned height)
    : core::layer(id, width, height, new matrix_cell_factory)
{
}

matrix_layer::~matrix_layer()
{}

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
