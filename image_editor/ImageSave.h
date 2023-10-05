#ifndef IMAGESAVE_H
#define IMAGESAVE_H

#include <QString>
#include <QHash>
#include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>

struct ImageSave
{
    QString filename;
    cv::Mat image;
    int width;
    int height;

    // TODO: Load more details of the image; camera, camera settings, etc...

    QHash<QString, int> effects;

    // TODO: How to store effects and the default values for the effects? For example: "rotate, default:0, applied:0"
    //          Make a new header with a struct for effects and a list of those effects? Hardcoded?
};

#endif // IMAGESAVE_H
