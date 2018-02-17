#include "layers_gallery.hpp"

#include "cells_window.hpp"

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
        cells_window* cw = new cells_window;
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

}
