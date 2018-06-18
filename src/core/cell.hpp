#ifndef CELL_HPP
#define CELL_HPP

#include <string>
#include <vector>

namespace core
{

class layer;
class submatrix;

struct index
{
    index(unsigned r, unsigned c)
        : m_row(r), m_column(c)
    {}
    unsigned m_row;
    unsigned m_column;
};

class cell
{
public:
    cell(unsigned r, unsigned c, const layer* l);
    virtual ~cell();

private:
    const unsigned m_row;
    const unsigned m_column;
    const layer* m_layer;

public:
    double value() const;
    void set_value(double);

public:
    //TODO
    double value_1() const;
    void set_value_1(double);

public:
    void dump(std::string&) const;

private:
    void dump_I(std::string&) const;
    void dump_R(std::string&) const;
    void dump_C(std::string&) const;
    void dump_Rsub(std::string&) const;
    void dump_R_right(std::string&) const;
    void dump_R_bottom(std::string&) const;
    void dump_r_right(std::string&) const;
    void dump_r_bottom(std::string&) const;

public:
    void add_submatrix(submatrix*);
    void notify_all();

private:
    using observers = std::vector<submatrix*>;
    observers m_observers;

private:
    double m_value;
    double m_value_1;
};

class cell_factory
{
public:
    virtual core::cell* create_cell(unsigned r, unsigned c, core::layer* l)
    {
        return new core::cell(r, c, l);
    }

};

}

#endif // CELL_HPP
