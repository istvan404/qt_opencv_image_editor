#include "ImageModel.h"

ImageModel::ImageModel(ImagePersistenceInterface* persistence, QObject *parent)
    : QObject{parent}
{
    _persistence = persistence;
}

bool ImageModel::loadImage(QString path)
{
    this->_data = _persistence->load(path);

    emit imageLoaded();
    return true;
}

QPixmap ImageModel::getQPixmap()
{
    QImage qimg(this->_data.image.data,
               this->_data.image.cols,
               this->_data.image.rows,
               static_cast<int>(this->_data.image.step),
               QImage::Format_RGB888);

    return QPixmap::fromImage(qimg);
}
