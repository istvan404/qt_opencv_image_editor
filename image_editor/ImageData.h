#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <QString>
#include <QHash>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

struct ImageData
{
    ImageData() {}
    ImageData(QString path_, cv::Mat image_)
    {
        this->path = path_;
        this->image = image_.clone();
        this->imageOriginal = image.clone();
    }

    QString path;
    QString filename;
    cv::Mat image;
    cv::Mat imageOriginal;
    int width;
    int height;

    // TODO: Load more details of the image; camera, camera settings, etc...

    QHash<QString, int> effects;

    // TODO: How to store effects and the default values for the effects? For example: "name:rotate, default:0, range:{0, 360}, applied:0"
    //          Make a new header with a struct for effects and add new effects to a QHash? Hardcoded?
    //              QHash<ImageEffects, bool>   ->  bool for toggle on and off? Maybe the view will make a list of toggled effects?
    //              Or just a QSet<ImageEffects> and the ImageEffects already has the applied and default values...
};

#endif // IMAGEDATA_H
