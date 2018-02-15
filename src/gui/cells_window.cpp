#include "cells_window.hpp"

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>

namespace gui
{

cells_window::cells_window(QWidget *p)
    : QWidget(p)
{
    init();
}

void cells_window::init()
{
    QHBoxLayout* l = new QHBoxLayout();
    m_scene = new QGraphicsScene();
    m_view = new QGraphicsView(m_scene);
    l->addWidget(m_view);
    setLayout(l);
}

void cells_window::fill_data(const files_parser::parser::power_cells & cells)
{
    for (auto i = cells.begin(); i != cells.end(); ++i) {
        QRectF r(QPoint((*i).pos().first, (*i).pos().second),
                 QSize(qreal((*i).width()), qreal((*i).height())));
        QGraphicsRectItem* ri = m_scene->addRect(r); // TODO add QPen and QGrush
        ri->setToolTip(QString::fromStdString((*i).name()));
    }
}

}
