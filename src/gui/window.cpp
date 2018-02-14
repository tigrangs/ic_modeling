#include "window.hpp"

#include <core/layer.hpp>

#include <QHBoxLayout>
#include <QMessageBox>
#include <QScreen>

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

    m_proxy = new QSurfaceDataProxy();
    m_series = new QSurface3DSeries(m_proxy);
    m_surface->addSeries(m_series);
    m_series->setDrawMode(QSurface3DSeries::DrawSurfaceAndWireframe);
//    m_series->setFlatShadingEnabled(true);
    m_surface->axisX()->setLabelAutoRotation(30);
    m_surface->axisY()->setLabelAutoRotation(90);
    m_surface->axisZ()->setLabelAutoRotation(30);

    QLinearGradient gr;
    gr.setColorAt(0.0, Qt::darkGreen);
    gr.setColorAt(0.5, Qt::yellow);
    gr.setColorAt(0.8, Qt::red);
    gr.setColorAt(1.0, Qt::darkRed);

    m_surface->seriesList().at(0)->setBaseGradient(gr);
    m_surface->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
    /*m_surface->axisX()->setLabelFormat("%.2f");
    m_surface->axisZ()->setLabelFormat("%.2f");*/
    QHBoxLayout* l = new QHBoxLayout();
    l->setMargin(0);
    l->addWidget(container);
    setLayout(l);
}

void thermal_window::fill_data(core::layer* l)
{
    QSurfaceDataArray *dataArray = new QSurfaceDataArray;
    dataArray->reserve(l->height());
    double max_value = 0;
    for (int i = 0 ; i < l->height() ; ++i) {
        QSurfaceDataRow *newRow = new QSurfaceDataRow(l->width());
        int index = 0;
        for (int j = 0; j < l->width(); ++j) {
            double v = l->get_cell_value(i, j);
            if (max_value < v) {
                max_value = v;
            }
            (*newRow)[index++].setPosition(QVector3D(2*i, 2*v, 2*j));
        }
        *dataArray << newRow;
    }
    m_surface->axisX()->setRange(0, l->width());
    m_surface->axisY()->setRange(0, max_value);
    m_surface->axisZ()->setRange(0, l->height());
    m_proxy->resetArray(dataArray);
}

}
