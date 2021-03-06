#include "cells_window.hpp"

#include <core/layer.hpp>
#include <controller/matrix_cell.hpp>
#include <controller/matrix_layer.hpp>

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QtMath>

#include <QDebug>

#include <cassert>
#include <iostream>

namespace gui
{

static const int POWER = 0;
static const int FREQUENCY = 1;

inline qreal round(qreal val, int step)
{
    int tmp = int(val) + step/2;
    tmp -= tmp % step;
    return qreal(tmp);
}

class scene : public QGraphicsScene
{
public:
    void show_grid(bool b)
    {
        m_show_grid = b;
        update();
    }
public:
    void set_grid_size(int s)
    {
        m_grid_step = s;
        update();
    }

public:
    int get_grid_size() const
    {
        return m_grid_step;
    }

protected:
    void drawForeground(QPainter *painter, const QRectF &rect) override
    {
        if (!m_show_grid) {
            return;
        }
        /*QRectF r = rect;
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(QPen(Qt::white,Qt::SolidLine));
        int max_X = r.width();
        int max_y = r.height();
        qDebug()<<r;
        for(int i = r.x() + cells_window::s_grid_step; i< max_X ; i += cells_window::s_grid_step) {
            painter->drawLine(QPointF(r.y() + cells_window::s_grid_step, i), QPointF(max_y, i));
        }
        for(int j = r.y() + cells_window::s_grid_step; j< max_y; j += cells_window::s_grid_step) {
            painter->drawLine(QPointF(j, r.x() + cells_window::s_grid_step), QPointF(j, max_X));
        }*/
        int step = m_grid_step;
        painter->setPen(QPen(QColor(Qt::white)));

        // draw horizontal grid.
        qreal start = round(rect.top(), step);
        if (start > rect.top()) {
            start -= step;
        }

        for (qreal y = start-step; y < rect.bottom();) {
            y += step;
            painter->drawLine(rect.left(), y, rect.right(), y);
        }

        // Draw vertical grid.
        start = round(rect.left(), step);
        if (start > rect.left()) {
            start -= step;
        }

        for (qreal x = start-step; x < rect.right();) {
            x += step;
            painter->drawLine(x, rect.top(), x, rect.bottom());
        }
    }

private:
    bool m_show_grid = false;
    int m_grid_step = 1;
};


cells_window::cells_window(unsigned id, QWidget *p)
    : QWidget(p)
    , m_id(id)
{
    init();
}

void cells_window::init()
{
    QHBoxLayout* l = new QHBoxLayout();
    m_scene = new scene;
    m_view = new QGraphicsView(m_scene);
    l->addWidget(m_view);
    setLayout(l);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    m_view->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

//    m_view->setBackgroundBrush(QBrush(QColor(240,240,240,82)/*, Qt::Dense1Pattern)*/));
    m_view->setBackgroundBrush(QBrush(Qt::gray));
}

void cells_window::fill_data(const files_parser::parser::power_cells & cells)
{
    double maxPow = 0;
    foreach (auto i, cells) {
        if (maxPow < i.power()) {
            maxPow = i.power();
        }
    }
    assert (maxPow != 0);

    foreach (auto i, cells) {
        QRectF r(QPoint(i.pos().first * 15, i.pos().second * 15),
                 QSize(qreal(i.width() * 15), qreal(i.height() * 15)));
        QPen pen;
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(2);
        pen.setColor(QColor(Qt::white));
        QColor color;
        qreal h = i.power()/maxPow * 256;
        color.setHsl(256 - h, 240, 140);
        QBrush br(color);
        QGraphicsRectItem* ri = m_scene->addRect(r, pen, br); // TODO add QPen and QBrush
/*        QGraphicsItem* ti = m_scene->addText(QString::fromStdString(i.name()), QFont("Times", 30));
        ti->setPos(r.center());*/
        ri->setToolTip(QString::fromStdString(i.name()));
        ri->setData(POWER, i.power());
        ri->setData(FREQUENCY, i.frequency());
        ri->setFlags(QGraphicsItem::ItemIsMovable |
                     QGraphicsItem::ItemIsSelectable |
                     QGraphicsItem::ItemSendsGeometryChanges |
                     QGraphicsItem::ItemSendsScenePositionChanges);
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
    m_scene->setSceneRect(br);
}

void cells_window::fit()
{
    QRectF br = m_scene->itemsBoundingRect();

/*    QRectF sr = m_view->contentsRect();
    qreal blength = br.width() < br.height() ? br.height() : br.width();
    qreal length = sr.width() < sr.height() ? sr.height() : sr.width();
    qreal length_factor = length/(blength*1.5);*/

//    m_view->scale(1/length_factor,1/length_factor);
    m_view->setAlignment(Qt::AlignCenter);
    m_view->centerOn(br.center().x(), br.center().y());
    m_scene->setSceneRect(br);
}

void cells_window::show_grid(bool s)
{
    assert(m_scene != 0);
    static_cast<scene*>(m_scene)->show_grid(s);
}

void cells_window::set_grid_size(int s)
{
    assert(m_scene != 0);
    m_size = s;
    static_cast<scene*>(m_scene)->set_grid_size(s);
}

int cells_window::get_grid_size() const
{
    return m_size;
}

const QGraphicsScene* cells_window::get_scene() const
{
    return m_scene;
}

void cells_window::dump_netlist(std::string& netlist)
{
    if (m_id == 0) {
        dump_defined_values(netlist);
    }
    dump_cells(netlist);
}

void cells_window::dump_defined_values(std::string& netlist)
{
    scene* sc = static_cast<scene*>(m_scene);
    assert(sc != 0);
    int step = sc->get_grid_size();

    ///////////////////////////     Rij       //////////////////////////////////////////
    const qreal lambda = 0.000233;
    qreal subThickness = 2.88;
    qreal h = 23.8;
    const qreal C = 20.16;
    const qreal r = 2329;

    qreal S = qPow(step, 2);
    qreal Rij = 1 / (lambda * h);
    netlist += QString(".param Rij = %1\n").arg(Rij).toStdString();

    ///////////////////////////     rij       //////////////////////////////////////////
    S = step * subThickness;
    qreal rij = 1 / (lambda * subThickness);
    netlist += QString(".param rij = %1\n").arg(rij).toStdString();

    ///////////////////////////     Ri       //////////////////////////////////////////
    S = qPow(step, 2);
    qreal Ri = h / (lambda * S);

    netlist += QString(".param Ri = %1\n").arg(Ri).toStdString();

    ///////////////////////////     Rsub       //////////////////////////////////////////
    S = qPow(step, 2);
    qreal Rsub =  subThickness/ (lambda * S);

    netlist += QString(".param Rsub = %1\n").arg(Rsub).toStdString();

    ///////////////////////////     Ci       //////////////////////////////////////////
    S = qPow(step, 2);
    qreal Ci =  C*r*S*h;
    Ci = 600; //forced

    netlist += QString(".param Ci = %1p\n").arg(Ci).toStdString();
}

core::layer* cells_window::get_layer(int itStep, int offset)
{
    assert(m_scene != 0);
    QRectF bRect = m_scene->itemsBoundingRect();
    QPointF distPoint = QPoint(offset, offset);
    QRectF cbRect(bRect.topLeft()-distPoint, bRect.bottomRight()+distPoint);

    qreal xStart = cbRect.topLeft().x();
    qreal xEnd = cbRect.topRight().x()-itStep;
    qreal yStart = cbRect.topLeft().y();
    qreal yEnd = cbRect.bottomLeft().y()-itStep;


    unsigned w = static_cast<unsigned>((xEnd - xStart) / itStep) + 1;
    unsigned h = static_cast<unsigned>((yEnd - yStart) / itStep) + 1;
    qDebug()<<QString::fromStdString("matrix size");
    qDebug()<<QString::number(w);
    qDebug()<<QString::number(h);
    core::layer* layer = new controller::matrix_layer(m_id, h, w);
    //core::layer* layer = new core::layer(m_id, h, w);
    std::cout<<"created_matrix_layer"<<std::endl;
    qreal theIPower =0.2;

    int row = 0;
    for(qreal x = xStart; x <= xEnd; x += itStep) {
        int column = 0;
        for(qreal y = yStart; y <= yEnd; y += itStep) {
            QRectF gridRect(x, y, itStep, itStep);
            QGraphicsRectItem* gridItem = new  QGraphicsRectItem(gridRect);

            m_scene->addItem(gridItem);
            qreal intersectP = 0;
            qreal intersectFR = 0;

            double max_value = 0;
            QList<QGraphicsItem *>  collidingItemsList = gridItem->collidingItems();
            QGraphicsRectItem* max_item = collidingItemsList.size() != 0 ? dynamic_cast<QGraphicsRectItem*>(collidingItemsList[0]) : 0;
            foreach(QGraphicsItem* item, collidingItemsList) {
                QGraphicsRectItem* ri = dynamic_cast<QGraphicsRectItem*>(item);
                if (ri) {
                    QRectF bRect = ri->boundingRect();
                    QRectF intersectRect = gridItem->rect().intersected(bRect);
                    QVariant v = ri->data(POWER);
                    assert(v.isValid());
                    double pv = v.toDouble();
                    ///intersectP += intersectRect.height() * intersectRect.width() * node->getPower() / S;

                    double value = intersectRect.height() * intersectRect.width() * pv /(gridItem->rect().height()* gridItem->rect().width());
                    if (value > max_value) {
                        max_value = value;
                        max_item = ri;
                    }
                    intersectP += value;
                    //333 intersectP += intersectRect.height() * intersectRect.width() * node->getPower();

                    v = ri->data(FREQUENCY);
                    assert(v.isValid());
                    double fr = v.toDouble();
                    if (fr < 0) {
                        continue;
                    }
                    intersectFR += intersectRect.height() * intersectRect.width() * fr /(gridItem->rect().height()* gridItem->rect().width());
                }
            }
            m_scene->removeItem(gridItem);

            if (!intersectP) {
                intersectP = theIPower/2;
            } else {
               theIPower = intersectP;
            }
            std::cout<<"set_value "<<row<<" "<<column<<std::endl;
            layer->set_cell_value(row, column, intersectP);
            layer->set_cell_value_1(row, column, intersectFR == 0 ? -1 : intersectFR);
            std::shared_ptr<controller::matrix_cell> cell =
                    std::static_pointer_cast<controller::matrix_cell>(layer->get_cell(row, column));
            if (max_item != 0) {
                /*qDebug()<<QString::number(column);
                qDebug()<<QString::number(row);
                qDebug()<<QString::fromStdString("ITEM_was_added");*/
                cell->add_item(max_item);
            }
            cell->set_source_position(row, column);
            ++column;
        }
        ++row;
    }
    return layer;
}

void cells_window::dump_cells(std::string& netlist)
{
    scene* sc = static_cast<scene*>(m_scene);
    assert(sc != 0);

    core::layer* layer = get_layer(sc->get_grid_size());
    assert(layer != 0);
    layer->dump(netlist);
    delete layer;
}

}
