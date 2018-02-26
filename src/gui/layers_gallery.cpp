#include "layers_gallery.hpp"

#include "cells_window.hpp"

#include <core/ic.hpp>

#include <QGraphicsScene>
#include <QGridLayout>
#include <QMultiHash>
#include <QtAlgorithms>

#include <cassert>

namespace gui
{

layers_gallery::layers_gallery(QWidget *parent)
    : QWidget(parent)
{
    m_layout = new QGridLayout;
    setLayout(m_layout);
//    m_layout->setSizeConstraint(QLayout::SetFixedSize);
//    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void layers_gallery::fill_layers(const files_parser::parser::power_cells& cells)
{
    cleanup_layers();
    add_layers(cells);
    layout_layers();
}

void layers_gallery::add_layers(const files_parser::parser::power_cells& cells)
{
    QMultiHash<unsigned, files_parser::power_cell> hash;
    foreach (auto c, cells) {
        hash.insertMulti(c.layer(), c);
    }
    QList<unsigned> layers_ids = hash.uniqueKeys();
    qSort(layers_ids);
    foreach (unsigned i, layers_ids) {
        cells_window* cw = new cells_window(i);
        QList<files_parser::power_cell> cls = hash.values(i);
        files_parser::parser::power_cells fppc;
        foreach (auto d, cls) {
            fppc.push_back(d);
        }
        cw->fill_data(fppc);
        m_layers.push_back(cw);
    }
}

void layers_gallery::cleanup_layers()
{
    foreach (auto l, m_layers) {
        l->deleteLater();
    }
    m_layers.clear();
}

void layers_gallery::layout_layers()
{
    assert(m_layout != 0);
    int r = 0;
    int c = 0;

    foreach (auto l, m_layers) {
        assert(l != 0);
        m_layout->addWidget(l, r, c);
        if (c + 1 < 2) {
            ++c;
        } else {
            c = 0;
            ++r;
        }
    }
}

void layers_gallery::show_grid(bool s)
{
    foreach (auto l, m_layers) {
        assert(l != 0);
        l->show_grid(s);
    }
}

void layers_gallery::set_grid_size(int s)
{
    foreach (auto l, m_layers) {
        assert(l != 0);
        l->set_grid_size(s);
    }
}

std::string layers_gallery::dump_netlist()
{
    // TODO
    std::string content;
    content += "***********************************************************************\n";
    content += "*******************Created by ic_modeling tool ************************\n";
    content += "***********************************************************************\n\n\n";
    content += ".prot\n";
    content += ".lib '/remote/u/tigrangs/hspice/saed32nm.lib' TT\n";
    content += ".unprot\n\n";
    content += ".temp 25\n";
    content += "vvdd vdd gnd dc = 1.2\n";
    content += "vvss vss gnd dc = 0.0\n";
    content += "***********************************************************************\n";
    foreach (auto l, m_layers) {
        assert(l != 0);
        l->dump_netlist(content);
    }
    content += "\n\n.option post probe\n\n";
    content += ".global gnd\n";
    content += ".probe v(*) i(*)\n";
    content += ".tran 10p 5n\n\n";
    content += ".end";
    return content;
}

QRectF layers_gallery::get_rect() const
{
    return m_layers[0]->get_scene()->itemsBoundingRect();
}

int layers_gallery::get_grid_size() const
{
    return m_layers[0]->get_grid_size();
}

core::ic* layers_gallery::get_ic()
{
    std::vector<core::layer*> layers;
    foreach (auto l, m_layers) {
        assert(l != 0);
        layers.push_back(l->get_layer(5));
    }
    return new core::ic(layers);
}

}
