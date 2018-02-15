#include "thermal_window.hpp"

#include <core/layer.hpp>

#include <QHBoxLayout>
#include <QMessageBox>
#include <QScreen>

#include <iostream>

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>

namespace gui
{

thermal_window::thermal_window(QWidget *parent) : QWidget(parent)
{
    init();
}

void thermal_window::init()
{
    m_surface = new Q3DSurface();
    QWidget *container = QWidget::createWindowContainer(m_surface);

    if (!m_surface->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return;
    }

    QSize screenSize = m_surface->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    m_surface->setAxisX(new QValue3DAxis);
    m_surface->setAxisY(new QValue3DAxis);
    m_surface->setAxisZ(new QValue3DAxis);

    m_series = new QSurface3DSeries();
    QHBoxLayout* l = new QHBoxLayout();
    l->setMargin(0);
    l->addWidget(container);
    setLayout(l);

    core::layer*  m_layer = new core::layer(0, 200, 200);
    for (unsigned i = 0; i< m_layer->height(); ++i) {
        for (unsigned j = 0; j < m_layer->width(); ++j) {
            m_layer->set_cell_value(i, j, i+j);
        }
    }
    fill_data(m_layer);
}

void thermal_window::fill_data(core::layer* l)
{
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    std::cout<<l->height()<<std::endl;
    dataArray->reserve(2*l->height()); //QT_BUG
    double max_value = 0;
    for (unsigned i = 0 ; i < 2*l->height() ; ++i) { //QT_BUG
        QSurfaceDataRow *newRow = new QSurfaceDataRow;
        for (unsigned j = 0; j < 2*l->width(); ++j) { //QT_BUG
            if (i >= l->height() || j >= l->width()) {
                (*newRow) << QVector3D(i, 0, j);
            } else {
                double v = l->get_cell_value(i, j);
                if (max_value < v) {
                    max_value = v;
                }
                (*newRow) << QVector3D(i, v, j);
            }
        }
        *dataArray << newRow;
    }
    std::cout<<dataArray->size()<<std::endl;

    m_series->dataProxy()->resetArray(dataArray);

    m_series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
//    m_series->setFlatShadingEnabled(true);

    m_surface->axisX()->setLabelFormat("%.2f");
    m_surface->axisZ()->setLabelFormat("%.2f");

    m_surface->axisX()->setRange(0, l->width() - 1);
    m_surface->axisY()->setRange(0, max_value);
    m_surface->axisZ()->setRange(0, l->height() - 1);

    m_surface->axisX()->setLabelAutoRotation(30);
    m_surface->axisY()->setLabelAutoRotation(90);
    m_surface->axisZ()->setLabelAutoRotation(30);



    m_surface->addSeries(m_series);

    m_surface->setSelectionMode(QAbstract3DGraph::SelectionItem);
    m_surface->activeTheme()->setType(Q3DTheme::Theme(2));

    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);
    m_surface->seriesList().at(0)->setBaseGradient(gr);
    m_surface->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

}

}
