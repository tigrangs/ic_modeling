#ifndef CELL_HPP
#define CELL_HPP

#include <string>

namespace core
{

class layer;

class cell
{
public:
    cell(unsigned r, unsigned c, const layer* l);

private:
    unsigned m_row;
    unsigned m_column;
    const layer* m_layer;

public:
    int value() const;
    void set_value(int);

public:
    void dump(std::string&) const;

private:
    void dump_R(std::string&) const;
    void dump_Rsub(std::string&) const;
    void dump_R_right(std::string&) const;
    void dump_R_bottom(std::string&) const;
    void dump_r_right(std::string&) const;
    void dump_r_bottom(std::string&) const;

private:
    int m_value;
};

}

#endif // CELL_HPP
