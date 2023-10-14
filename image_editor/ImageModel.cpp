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

QPixmap ImageModel::getEditedImageQPixmap(QSize imageLabelSize)
{
    cv::Mat img = this->_data.image;
    float inW = img.cols;
    float inH = img.rows;
    float outW = imageLabelSize.width();
    float outH = imageLabelSize.height();
    float newW;
    float newH;

    float aspectRatio = (float)inW / (float)inH;
    int interpolation = cv::INTER_CUBIC; // assuming we are stretching the image

    if(inW > outW || inH > outH) // shrinking image
        interpolation = cv::INTER_AREA;

    if(aspectRatio > 1) // horizontal image
    {
        newW = outW;
        newH = newW / aspectRatio;
    }
    else if(aspectRatio < 1) // vertical image
    {
        newH = outH;
        newW = newH*aspectRatio;
    }
    // else square image
    else // square image
    {
        newW = outW;
        newH = outH;
        // nothing to do because newW and newH are already equal to outW and outH
    }

    cv::resize(img, img, cv::Size(newW, newH), 0, 0, interpolation);

    qDebug() << "w : " << inW << " -> " << newW;
    qDebug() << "h : " << inH << " -> " << newH;

    /* Sharpening
    cv::Mat imgGaussianBlur;
    cv::GaussianBlur(img, imgGaussianBlur, cv::Size(0,0), 3);
    cv::addWeighted(img, 1.5, imgGaussianBlur, -0.5, 0, img);*/

    QImage qimg(img.data,
                img.cols,
                img.rows,
                static_cast<int>(img.step),
                QImage::Format_RGB888);

    return QPixmap::fromImage(qimg.rgbSwapped());
}

void ImageModel::editFlipHorizontal()
{
    cv::flip(this->_data.image, this->_data.image, 0);
    emit imageLoaded();
}

void ImageModel::editFlipVertical()
{
    cv::flip(this->_data.image, this->_data.image, 1);
    emit imageLoaded();
}

void ImageModel::editRotate90Plus()
{
    cv::rotate(this->_data.image, this->_data.image, cv::ROTATE_90_CLOCKWISE);
    emit imageLoaded();
}

void ImageModel::editRotate90Minus()
{
    cv::rotate(this->_data.image, this->_data.image, cv::ROTATE_90_COUNTERCLOCKWISE);
    emit imageLoaded();
}
