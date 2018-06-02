#include "matrix_layer.hpp"
#include "matrix_cell.hpp"

namespace controller {

matrix_layer::matrix_layer(unsigned id, unsigned width, unsigned height)
    : core::layer(id, width, height)
{
}

core::cell* matrix_layer::create_cell(unsigned r, unsigned c)
{
    return new matrix_cell(r, c, this);
}

}
