#ifndef SUBMATRIX_HPP
#define SUBMATRIX_HPP

#include "cell.hpp"

#include <memory>
#include <vector>

namespace core {

class layer;

class submatrix
{
public:
    submatrix(layer* layer,const unsigned size, index idx);

    unsigned get_sum() const;

public:
    void invalidate();

    std::shared_ptr<cell> get_max_cell();
    std::shared_ptr<cell> get_min_cell();

public:
    using cells = std::vector<std::shared_ptr<cell>>;
    cells& get_cells();

private:
    void recalculate_sum() const;

private:
    layer* m_layer;
    const unsigned m_size;
    cells m_cells;
    index m_index;
    mutable bool m_sum_is_valid;
    mutable unsigned m_sum;
};

}

#endif // SUBMATRIX_HPP
