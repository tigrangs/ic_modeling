#ifndef MATRIX_LAYER_HPP
#define MATRIX_LAYER_HPP

#include <core/layer.hpp>
#include <core/submatrix.hpp>

namespace controller {

class matrix_layer : public core::layer
{
public:
    matrix_layer(unsigned id, unsigned width, unsigned height);

public:
    void generate_submatrixes(const unsigned size);

private:
    core::cell* create_cell(unsigned r, unsigned c) override;

public:
    using submatrixes = std::vector<std::vector<core::submatrix*>>;

    submatrixes& get_submatrixes();
private:
    submatrixes m_submatrixes;
};

}

#endif // MATRIX_LAYER_HPP
