#include "Surface.h"

void Surface::BuildScene(Q3DSurface* surface)
{
    surface->setHorizontalAspectRatio(1.0);
    surface->axisX()->setTitle("Observation point, m");
    surface->axisY()->setTitle("SE, dB");
    surface->axisZ()->setTitle("Frequency, MHz");
    surface->axisZ()->setLabelFormat("%.1f");

    surface->axisX()->setLabelAutoRotation(30.0f);
    surface->axisY()->setLabelAutoRotation(30.0f);
    surface->axisZ()->setLabelAutoRotation(30.0f);

    surface->axisX()->setTitleVisible(true);
    surface->axisY()->setTitleVisible(true);
    surface->axisZ()->setTitleVisible(true);

    surface->activeTheme()->setWindowColor(QColor(QRgb(0xf0f0f0)));
}

void Surface::BuildSurface(Q3DSurface* surface, int ptsNum, int strNum, double* f, double* p, double** SE)
{
	double x, y, z;
    QSurfaceDataArray* data = new QSurfaceDataArray;
    QSurface3DSeries* series = new QSurface3DSeries;
    QLinearGradient gr;

    for (int i = 0; i < strNum; i++)
    {
        QSurfaceDataRow* dataRow = new QSurfaceDataRow(ptsNum);
        z = f[i] / pow(10, 6);
        for (int j = 0; j < ptsNum; j++)
        {
            x = p[j];
            y = SE[i][j];
            (*dataRow)[j].setPosition(QVector3D(x, y, z));
        }
        *data << dataRow;
    }
    series->dataProxy()->resetArray(data);
    series->setDrawMode(QSurface3DSeries::DrawSurface);
    surface->addSeries(series);

    gr.setColorAt(0.0, Qt::black);
    gr.setColorAt(0.3, Qt::blue);
    gr.setColorAt(0.6, Qt::red);

    surface->seriesList().at(0)->setBaseGradient(gr);
    surface->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyleRangeGradient);
}
