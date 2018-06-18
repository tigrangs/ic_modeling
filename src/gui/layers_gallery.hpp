#ifndef LAYERS_GALLERY_HPP
#define LAYERS_GALLERY_HPP

#include <parser/parser.hpp>

#include <QList>
#include <QWidget>

class QGridLayout;

namespace core
{

class ic;

}

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
    int get_grid_size() const;
    QRectF get_rect() const;
    std::string dump_netlist();
    core::ic* get_ic();

    void fit();

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
