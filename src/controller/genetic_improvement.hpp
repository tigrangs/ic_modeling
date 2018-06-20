#ifndef GENETIC_IMPROVEMENT_HPP
#define GENETIC_IMPROVEMENT_HPP

#include "matrix_layer.hpp"

namespace controller {

class genetic_improvement
{
public:
    genetic_improvement(matrix_layer* l, unsigned size);

public:
    matrix_layer *run();

private:
    void swap_items(matrix_layer::submatrixes&);
    void check_improvement(matrix_layer::submatrixes&);
    double remove_matrixes();

private:
    matrix_layer* m_root_matrix;
    unsigned m_submatrix_size;
    double m_max_value;
    bool m_stop;
    unsigned count;
    using matrixes=std::vector<matrix_layer*>;
    matrixes m_parents;
};

}

#endif // GENETIC_IMPROVEMENT_HPP
