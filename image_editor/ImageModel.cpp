#include "ImageModel.h"

ImageModel::ImageModel(ImagePersistenceInterface* persistence, QObject *parent)
    : QObject{parent}
{
    _persistence = persistence;
    _toggleImageFillSpace = true;
}

void ImageModel::loadImage(QString path)
{
    this->_data = _persistence->load(path);
    emit imageLoaded();
}

void ImageModel::saveImage(QString path)
{
    // Edge cases
    if(!this->isImageLoaded())
        return;

    _persistence->save(path, this->_data);
}

bool ImageModel::isImageLoaded()
{
    return this->_data != nullptr;
}

QPixmap ImageModel::getEditedImageQPixmap(QSize imageLabelSize)
{
    QSize imageSize = QSize( imageLabelSize.width()-10, imageLabelSize.height()-10 );

    cv::Mat img = this->resizeMatrix(this->_data->image, imageSize);
    //cv::Mat img = this->_data->image;

    QImage qimg(img.data,
                img.cols,
                img.rows,
                static_cast<int>(img.step),
                QImage::Format_RGB888);

    return QPixmap::fromImage(qimg.rgbSwapped());
}

cv::Mat ImageModel::resizeMatrix(cv::Mat input, QSize availableSize)
{
    // Edge cases
    if(!this->isImageLoaded())
        return input;

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

    cv::resize(img,
               img,
               cv::Size(),
               (newWidth / inWidth),
               (newHeight / inHeight),
               interpolation);

    return img;
}

QPixmap ImageModel::getHistogram(QSize histogramLabelSize)
{
    // Edge cases
    if(!this->isImageLoaded())
        return QPixmap();           // TODO: Revisit this solution to edge case, maybe EMIT some problem? THROW error?

    float width = histogramLabelSize.width()-10;
    float height = histogramLabelSize.height()-10;
    cv::Mat img(height, width, CV_8UC3, cv::Scalar(0,0,0));

    cv::line(img, cv::Point(0,0), cv::Point(0, 100), cv::Scalar(255,0,0),1);
    cv::line(img, cv::Point(0,0), cv::Point(100, 0), cv::Scalar(255,0,0),1);

    QImage qimg(img.data,
                img.cols,
                img.rows,
                static_cast<int>(img.step),
                QImage::Format_RGB888);

    return QPixmap::fromImage(qimg.rgbSwapped());
}

void ImageModel::editFlipHorizontal()
{
    // Edge cases
    if(!this->isImageLoaded())
        return;

    cv::flip(this->_data->image, this->_data->image, 0);
    emit imageLoaded();
}

void ImageModel::editFlipVertical()
{
    // Edge cases
    if(!this->isImageLoaded())
        return;

    cv::flip(this->_data->image, this->_data->image, 1);
    emit imageLoaded();
}

void ImageModel::editRotate90Plus()
{
    // Edge cases
    if(!this->isImageLoaded())
        return;

    cv::rotate(this->_data->image, this->_data->image, cv::ROTATE_90_CLOCKWISE);
    emit imageLoaded();
}

void ImageModel::editRotate90Minus()
{
    // Edge cases
    if(!this->isImageLoaded())
        return;

    cv::rotate(this->_data->image, this->_data->image, cv::ROTATE_90_COUNTERCLOCKWISE);
    emit imageLoaded();
}

void ImageModel::editToggleImageScale(bool toggle)
{
    // Edge cases
    if(!this->isImageLoaded())
        return;

   this->_toggleImageFillSpace = toggle;
   emit imageLoaded();
}

void ImageModel::editAutoWhiteBalance()
{
    // Edge cases
    if(!this->isImageLoaded())
        return;

    float percent = 1;
    //assert(in.channels() == 3);
    //assert(percent > 0 && percent < 100);
    float halfPercent = percent / 200.0f;

    std::vector<cv::Mat> bgrChannelSplit;
    cv::split(this->_data->image,bgrChannelSplit);
    for(int i=0;i<3;i++) {
        //find the low and high precentile values (based on the input percentile)
        cv::Mat monoImage;
        bgrChannelSplit[i].reshape(1,1).copyTo(monoImage);
        cv::sort(monoImage,monoImage,cv::SORT_EVERY_ROW + cv::SORT_ASCENDING);
        int lowval = monoImage.at<uchar>(cvFloor(((float)monoImage.cols) * halfPercent));
        int highval = monoImage.at<uchar>(cvCeil(((float)monoImage.cols) * (1.0 - halfPercent)));

        //saturate below the low percentile and above the high percentile
        bgrChannelSplit[i].setTo(lowval,bgrChannelSplit[i] < lowval);
        bgrChannelSplit[i].setTo(highval,bgrChannelSplit[i] > highval);

        //scale the channel
        cv::normalize(bgrChannelSplit[i],bgrChannelSplit[i],0,255,cv::NORM_MINMAX);
    }
    cv::merge(bgrChannelSplit,this->_data->image);

    emit imageLoaded();
}
