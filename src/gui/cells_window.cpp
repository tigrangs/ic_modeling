#include "cells_window.hpp"

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>

#include <QDebug>

#include <cassert>

namespace gui
{

static const int POWER = 0;

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
        start =  round(rect.left(), step);
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

cells_window::cells_window(QWidget *p)
    : QWidget(p)
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

    m_view->setBackgroundBrush(QBrush(Qt::black));
}

void cells_window::fill_data(const files_parser::parser::power_cells & cells)
{
    foreach (auto i, cells) {
        QRectF r(QPoint(i.pos().first * 100, i.pos().second * 100),
                 QSize(qreal(i.width() * 100), qreal(i.height() * 100)));
        QPen pen;
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(2);
        pen.setColor(QColor(Qt::green));
        QGraphicsRectItem* ri = m_scene->addRect(r, pen); // TODO add QPen and QBrush
/*        QGraphicsItem* ti = m_scene->addText(QString::fromStdString(i.name()), QFont("Times", 30));
        ti->setPos(r.center());*/
        ri->setToolTip(QString::fromStdString(i.name()));
        ri->setData(POWER, i.power());
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

void cells_window::show_grid(bool s)
{
    assert(m_scene != 0);
    static_cast<scene*>(m_scene)->show_grid(s);
}

void cells_window::set_grid_size(int s)
{
    assert(m_scene != 0);
    static_cast<scene*>(m_scene)->set_grid_size(s);
}

}
