#ifndef MATRIX_LAYER_HPP
#define MATRIX_LAYER_HPP

#include <core/layer.hpp>
#include <core/submatrix.hpp>

namespace controller {

class matrix_layer : public core::layer
{
public:
    matrix_layer(unsigned id, unsigned width, unsigned height);
    virtual ~matrix_layer();

public:
    void generate_submatrixes(const unsigned size);

public:
    using submatrixes = std::vector<std::vector<core::submatrix*>>;

    submatrixes& get_submatrixes();
private:
    submatrixes m_submatrixes;
};

}

#endif // MATRIX_LAYER_HPP
