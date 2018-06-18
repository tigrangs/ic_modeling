#ifndef GENETIC_IMPROVEMENT_HPP
#define GENETIC_IMPROVEMENT_HPP

#include "matrix_layer.hpp"

namespace controller {

class genetic_improvement
{
public:
    genetic_improvement(matrix_layer* l, unsigned size);

public:
    void run();

private:
    void swap_items(matrix_layer::submatrixes&);
    void check_improvement(matrix_layer::submatrixes&);

private:
    matrix_layer* m_root_matrix;
    unsigned m_submatrix_size;
    double m_max_value;
    bool m_stop;
    unsigned count;
};

}

#endif // GENETIC_IMPROVEMENT_HPP
