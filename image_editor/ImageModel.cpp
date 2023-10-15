#include "ImageModel.h"

ImageModel::ImageModel(ImagePersistenceInterface* persistence, QObject *parent)
    : QObject{parent}
{
    _persistence = persistence;
    _toggleImageFillSpace = true;
}

bool ImageModel::loadImage(QString path)
{
    this->_data = _persistence->load(path);

    emit imageLoaded();
    return true;
}

bool ImageModel::saveImage(QString path)
{
    return cv::imwrite(path.toStdString(), this->_data.image);
}

QPixmap ImageModel::getEditedImageQPixmap(QSize imageLabelSize)
{
    cv::Mat img = this->resizeMatrix(this->_data.image, imageLabelSize);

    QImage qimg(img.data,
                img.cols,
                img.rows,
                static_cast<int>(img.step),
                QImage::Format_RGB888);

    qimg = qimg.rgbSwapped();
    //qimg = qimg.scaled(imageLabelSize, Qt::KeepAspectRatio, Qt::FastTransformation);

    //return QPixmap::fromImage(qimg.rgbSwapped());
    return QPixmap::fromImage(qimg);
}

cv::Mat ImageModel::resizeMatrix(cv::Mat input, QSize availableSize)
{
    if(!this->_toggleImageFillSpace)
        return input;

    cv::Mat img = input;
    float inWidth = input.cols;
    float inHeight = input.rows;
    float outWidth = availableSize.width();
    float outHeight = availableSize.height();
    float newWidth = 0;
    float newHeight = 0;
    float aspectRatio = inWidth / inHeight;
    cv::InterpolationFlags interpolation;

    if( inWidth > outWidth || inHeight > outHeight )    // Shrinking image
        interpolation = cv::INTER_AREA;
    else                                                // Stretching image
        interpolation = cv::INTER_CUBIC;

    if(aspectRatio > 1)         // Horizontal image
    {
        newWidth = outWidth;
        newHeight = outWidth / aspectRatio;
    }
    else if(aspectRatio < 1)    // Vertical image
    {
        newHeight = outHeight;
        newWidth = outHeight * aspectRatio;
    }
    else                        // Square image
    {
        newWidth = outWidth;
        newHeight = outHeight;
    }

    //img = this->resizeMatrixBySteps(img, QSize(newWidth, newHeight), interpolation, 20);

    cv::resize(img,
               img,
               cv::Size(),
               (newWidth / inWidth),
               (newHeight / inHeight),
               interpolation);



    /* Sharpening */
    /*cv::Mat imgGaussianBlur;
    cv::GaussianBlur(img, imgGaussianBlur, cv::Size(0,0), 3);
    cv::addWeighted(img, 1.5, imgGaussianBlur, -0.5, 0, img);*/

    return img;
}


cv::Mat ImageModel::resizeMatrixBySteps(cv::Mat input, QSize targetSize, cv::InterpolationFlags interpolation, int steps)
{
    if(steps <= 1)
        return input;

    cv::Mat img = input;
    float inWidth = img.cols;
    float inHeight = img.rows;
    float outWidth = targetSize.width();
    float outHeight = targetSize.height();
    float stepWidth = 0;
    float stepHeight = 0;

    stepWidth = (inWidth - outWidth) / steps;
    stepHeight = (inHeight - outHeight) / steps;

    for(int i = 1; i < steps; i++)
    {
        // (int) conversion rounds down!!!
        qDebug() << "W => " << inWidth + (i*stepWidth) << "int -> " << (int)(inWidth + (i*stepWidth));
        qDebug() << "H => " << inHeight + (i*stepHeight) << "int -> " << (int)(inHeight + (i*stepHeight));
        cv::resize(img,
                   img,
                   cv::Size(inWidth + (i*stepWidth), inHeight + (i*stepHeight)),
                   0,
                   0,
                   interpolation);
    }

    return input;
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

void ImageModel::editToggleImageScale(bool toggle)
{
   this->_toggleImageFillSpace = toggle;
   emit imageLoaded();
}
