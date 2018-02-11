#ifndef CELL_HPP
#define CELL_HPP

namespace core
{

class cell
{
public:
    cell(unsigned r, unsigned c, unsigned l);

private:
    unsigned m_row;
    unsigned m_column;
    unsigned m_layer;

public:
    int value() const;
    void set_value(int);

private:
    int m_value;
};

}

#endif // CELL_HPP
