#ifndef MATRIX_CELL_HPP
#define MATRIX_CELL_HPP

#include <core/cell.hpp>
#include <core/layer.hpp>

#include <QGraphicsRectItem>

namespace controller {

class matrix_cell : public core::cell
{
public:
    matrix_cell(unsigned r, unsigned c, const core::layer* l);
    virtual ~matrix_cell();

public:
    void set_source_position(unsigned r, unsigned c);

public:
    void add_item(QGraphicsRectItem* item);
    QGraphicsRectItem* get_item();

public:
    unsigned source_row() const;
    unsigned source_column() const;

private:
    QGraphicsRectItem* m_item;

private:
    unsigned m_source_row = -1;
    unsigned m_source_column = -1;
};

}

#endif // MATRIX_CELL_HPP
