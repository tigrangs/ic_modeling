#ifndef LAYERS_GALLERY_HPP
#define LAYERS_GALLERY_HPP

#include <parser/parser.hpp>

#include <QList>
#include <QWidget>

class QGridLayout;

namespace gui
{

class cells_window;

class layers_gallery : public QWidget
{
    Q_OBJECT
public:
    explicit layers_gallery(QWidget *parent = nullptr);

public:
    void fill_layers(const files_parser::parser::power_cells&);
    void show_grid(bool s);
    void set_grid_size(int s);
    std::string dump_netlist();

private:
    void add_layers(const files_parser::parser::power_cells&);
    void cleanup_layers();
    void layout_layers();

private:
    using layers = QList<cells_window*>;

private:
    layers m_layers = {};

    QGridLayout* m_layout = 0;
};

}

#endif // LAYERS_GALLERY_HPP
