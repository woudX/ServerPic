#ifndef OPENCVEX_H
#define OPENCVEX_H

#include "stdafx.h"

class OpencvEx
{
public:
    OpencvEx();
    static void ResizePictureFile(string src, string opt, int width, int height);
    static void DrawText();
};

#endif // OPENCVEX_H
