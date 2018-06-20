#ifndef CELLS_WINDOW_HPP
#define CELLS_WINDOW_HPP

#include <parser/parser.hpp>

#include <QWidget>

class QGraphicsView;
class QGraphicsScene;

namespace core {

class layer;

}

namespace gui
{

class cells_window : public QWidget
{
public:
    cells_window(unsigned id, QWidget* p = nullptr);

private:
    void init();

public:
    void fill_data(const files_parser::parser::power_cells&);
    void show_grid(bool);
    void set_grid_size(int);
    int get_grid_size() const;
    const QGraphicsScene* get_scene() const;
    void dump_netlist(std::string&);
    core::layer* get_layer(int, int = 20);

public:
    void fit();

private:
    void dump_defined_values(std::string&);
    void dump_cells(std::string&);

private:
    QGraphicsView* m_view = 0;
    QGraphicsScene* m_scene = 0;
    unsigned m_id = 0;
    int m_size = 1;
};

}

#endif // CELLS_WINDOW_HPP
