#include "cells_window.hpp"

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>

#include <cassert>

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
    unsigned xmax = 0;
    unsigned ymax = 0;
    unsigned xmin = -1;
    unsigned ymin = -1;
    foreach (auto i, cells) {
        QRectF r(QPoint(i.pos().first * 100, i.pos().second * 100),
                 QSize(qreal(i.width() * 100), qreal(i.height() * 100)));
        unsigned x = i.pos().first + i.width();
        unsigned y = i.pos().second + i.height();
        if (x > xmax) {
            xmax = x;
        }
        if (y > ymax) {
            ymax = y;
        }

        if (i.pos().first < xmin) {
            xmin = i.pos().first;
        }
        if (i.pos().second < ymin) {
            ymin = i.pos().second;
        }
        QPen pen;
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(2);
        QGraphicsRectItem* ri = m_scene->addRect(r, pen); // TODO add QPen and QBrush
        QGraphicsItem* ti = m_scene->addText(QString::fromStdString(i.name()), QFont("Times", 30));
        ti->setPos(r.center());
        ri->setToolTip(QString::fromStdString(i.name()));
    }
    assert(m_scene != 0);
    assert(m_view != 0);
    QRectF br = m_scene->itemsBoundingRect();

    QRectF sr = m_view->contentsRect();
    qreal blength = br.width() < br.height() ? br.height() : br.width();
    qreal length = sr.width() < sr.height() ? sr.height() : sr.width();
    qreal length_factor = length/(blength*1.5);

    m_view->scale(length_factor,length_factor);
    m_view->setAlignment(Qt::AlignCenter);
    m_view->centerOn(br.center().x(), br.center().y());


//    m_scene->setSceneRect(xmin, ymin, xmax - xmin, ymax - ymin);
}

}
