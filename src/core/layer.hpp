#ifndef LAYER_HPP
#define LAYER_HPP

#include <memory>
#include <vector>
#include <string>

namespace core
{

class cell;
class cell_factory;

class layer
{
public:
    using cells = std::vector<std::vector<std::shared_ptr<cell>>>;

public:
    layer(unsigned id, unsigned width, unsigned height, cell_factory* f = 0);
    virtual ~layer();

public:
    unsigned id() const;
    unsigned width() const;
    unsigned height() const;

public:
    void set_cell_value(unsigned r, unsigned c, double v);
    double get_cell_value(unsigned r, unsigned c);
    void set_cell_value_1(unsigned r, unsigned c, double v);
    double get_cell_value_1(unsigned r, unsigned c);

public:
    std::shared_ptr<cell> get_cell(unsigned r, unsigned c);

public:
    void dump(std::string&) const;

private:
    void init(cell_factory*);

private:
    unsigned m_id;
    unsigned m_width;
    unsigned m_height;

private:
    cells m_cells;
};

}

#endif // LAYER_HPP
