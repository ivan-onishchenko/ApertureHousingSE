#pragma once

#include <QtDataVisualization/Q3DSurface>
using namespace QtDataVisualization;

class Surface
{
public:
    static void BuildScene(Q3DSurface* surface);
    static void BuildSurface(Q3DSurface* surface, int ptsNum, int strNum, double* f, double* p, double** SE);
};
