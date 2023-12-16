#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <QString>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

struct ImageData
{
    QString Path;
    cv::Mat Image;
    cv::Mat ImageOriginal;

    ImageData() {}
    ImageData(QString path, cv::Mat image)
    {
        Path = path;
        Image = image.clone();
        ImageOriginal = image.clone();
    }
};

#endif // IMAGEDATA_H
