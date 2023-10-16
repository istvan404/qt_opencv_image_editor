#include "ImagePersistence.h"
#include <QString>


ImagePersistence::ImagePersistence()
{
}

void ImagePersistence::save(QString path, ImageData* state)
{
    cv::imwrite(path.toStdString(), state->image);
}

ImageData* ImagePersistence::load(QString path)
{
    cv::Mat image = cv::imread(path.toStdString());
    return new ImageData(path, image);
}
