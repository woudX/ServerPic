#include "opencvex.h"

OpencvEx::OpencvEx()
{

}

void OpencvEx::ResizePictureFile(string src, string opt, int width, int height)
{
    Mat inputPic = imread(src.c_str(), IMREAD_COLOR);
    Mat outputPic;

    //  calculate best factor
    double fx =  width / (double)(inputPic.cols);
    double fy = height / (double)(inputPic.rows);
    double factor = max(fx, fy);

    //  resize pic
    if (fx > 1.0f || fy > 1.0f)
        outputPic = inputPic.clone();
    else
        resize(inputPic, outputPic, Size(0, 0), factor, factor, INTER_LANCZOS4);

    //  output
    imwrite(opt.c_str(), outputPic);

    //  release
    inputPic.release();
    outputPic.release();
}

