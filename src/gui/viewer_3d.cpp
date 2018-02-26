#include "viewer_3d.hpp"

#include <core/layer.hpp>

#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QScreen>

#include <cassert>

#include <iostream>

#include <QtDataVisualization/QValue3DAxis>
#include <QtDataVisualization/Q3DTheme>

namespace gui
{

viewer_3d::viewer_3d(QWidget *parent) : QWidget(parent)
{
    init();
}

void viewer_3d::init()
{
    m_surface = new Q3DSurface();
    QWidget *container = QWidget::createWindowContainer(m_surface);

    if (!m_surface->hasContext()) {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
        return;
    }

    //QSize screenSize = m_surface->screen()->size();
    //container->setMinimumSize(QSize(screenSize.width() / 2, screenSize.height() / 1.6));
    //container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    m_surface->setAxisX(new QValue3DAxis);
    m_surface->setAxisY(new QValue3DAxis);
    m_surface->setAxisZ(new QValue3DAxis);

    m_series = new QSurface3DSeries();
    QVBoxLayout* l = new QVBoxLayout();
    m_label = new QLabel();
    l->setMargin(0);
    l->addWidget(m_label);
    l->addWidget(container);
    setLayout(l);
}

void viewer_3d::fill_data(core::layer* l)
{
    assert(m_label != 0);
    m_label->setText(QString::fromStdString("Layer " + std::to_string(l->id())));
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


    m_series->setDrawMode(m_draw_mode);
    //m_series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
//    m_series->setFlatShadingEnabled(true);

    m_surface->axisX()->setLabelFormat("%.2f");
    m_surface->axisZ()->setLabelFormat("%.2f");

    m_surface->axisX()->setRange(0, l->height() - 1);
    m_surface->axisY()->setRange(0, max_value);
    m_surface->axisZ()->setRange(0, l->width() - 1);

    m_surface->axisX()->setLabelAutoRotation(30);
    m_surface->axisY()->setLabelAutoRotation(90);
    m_surface->axisZ()->setLabelAutoRotation(30);



    m_surface->addSeries(m_series);

    m_surface->setSelectionMode(QAbstract3DGraph::SelectionItem);
    m_surface->activeTheme()->setType(Q3DTheme::Theme(2));
}

void viewer_3d::set_draw_mode(QSurface3DSeries::DrawFlag df)
{
    assert(m_series != 0);
    m_series->setDrawMode(df);
    m_draw_mode = df;
}


void viewer_3d::set_gradient(QLinearGradient gr)
{
    m_surface->seriesList().at(0)->setBaseGradient(gr);
    m_surface->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);

}

}
