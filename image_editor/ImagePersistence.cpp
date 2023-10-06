#include "ImagePersistence.h"
#include <QString>


ImagePersistence::ImagePersistence()
{

}

void ImagePersistence::save(QString path, ImageData state)
{

}

ImageData ImagePersistence::load(QString path)
{
    cv::Mat image = cv::imread(path.toStdString());

    return ImageData(path, image);
}
