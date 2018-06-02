#ifndef MATRIX_LAYER_HPP
#define MATRIX_LAYER_HPP

#include <core/layer.hpp>

namespace controller {

class matrix_layer : public core::layer
{
public:
    matrix_layer(unsigned id, unsigned width, unsigned height);

private:
    core::cell* create_cell(unsigned r, unsigned c) override;
};

}

#endif // MATRIX_LAYER_HPP
