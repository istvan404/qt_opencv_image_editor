#include "ImagePersistence.h"


ImagePersistence::ImagePersistence()
{
}

void ImagePersistence::save(QString path, ImageData* state)
{
    cv::imwrite(path.toStdString(), state->Image);
}

ImageData* ImagePersistence::load(QString path)
{
    cv::Mat image = cv::imread(path.toStdString(), cv::IMREAD_COLOR);
    return new ImageData(path, image);
}
