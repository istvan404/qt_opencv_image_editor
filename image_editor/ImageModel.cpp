#include "ImageModel.h"

ImageModel::ImageModel(ImagePersistenceInterface* persistence, QObject *parent)
    : QObject{parent}
{
    _persistence = persistence;
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

QPixmap ImageModel::getEditedImageQPixmap()
{
    cv::Mat img = this->_data->image;

    QImage qimg(img.data,
                img.cols,
                img.rows,
                static_cast<int>(img.step),
                QImage::Format_RGB888);

    return QPixmap::fromImage(qimg.rgbSwapped());
}

QPixmap ImageModel::getHistogram(QSize histogramLabelSize)
{
    // Edge cases
    if(!this->isImageLoaded())
        return QPixmap();           // TODO: Revisit this solution to edge case, maybe EMIT some problem? THROW error?

    int width = histogramLabelSize.width();
    if(width % 2 != 0)
        width++;
    int height = histogramLabelSize.height();
    if(height % 2 != 0)
        height++;

    cv::Mat img(height, width, CV_8UC3, cv::Scalar(10,10,10));

    img = generateHistogramGridOverlay(img, 6, 4);

    QImage qimg(img.data,
                img.cols,
                img.rows,
                static_cast<int>(img.step),
                QImage::Format_RGB888);

    return QPixmap::fromImage(qimg.rgbSwapped());
}

cv::Mat ImageModel::generateHistogramGridOverlay(cv::Mat source, int gridCols, int gridRows)
{
    cv::Mat img = source;
    int width = img.cols;
    int height = img.rows;
    int thickness = 1;
    cv::Scalar color = cv::Scalar(100,100,100);

    cv::rectangle(img, cv::Rect(cv::Point(0,0), cv::Point(width, height)), color, thickness*2);

    int verticalSpacing = (width - 2*thickness - gridCols*thickness) / (gridCols+1);
    for(int i = 1; i <= gridCols; i++)
    {
        int spacing = i*verticalSpacing + thickness + thickness*(i-1);
        cv::line(img,
                 cv::Point(spacing+1, 0),
                 cv::Point(spacing+1, height),
                 color,
                 thickness);
    }

    int horizontalSpacing = (height - 2*thickness - gridRows*thickness) / (gridRows+1);
    for(int i = 1; i <= gridRows; i++)
    {
        int spacing = i*horizontalSpacing + thickness + thickness*(i-1);
        cv::line(img,
                 cv::Point(0, spacing+1),
                 cv::Point(width, spacing+1),
                 color,
                 thickness);
    }

    return img;
}

void ImageModel::editFlipHorizontal()
{
    // Edge cases
    if(!this->isImageLoaded())
        return;

    cv::flip(this->_data->image, this->_data->image, 0);
    emit imageUpdated();
}

void ImageModel::editFlipVertical()
{
    // Edge cases
    if(!this->isImageLoaded())
        return;

    cv::flip(this->_data->image, this->_data->image, 1);
    emit imageUpdated();
}

void ImageModel::editRotate90Plus()
{
    // Edge cases
    if(!this->isImageLoaded())
        return;

    cv::rotate(this->_data->image, this->_data->image, cv::ROTATE_90_CLOCKWISE);
    emit imageUpdated();
}

void ImageModel::editRotate90Minus()
{
    // Edge cases
    if(!this->isImageLoaded())
        return;

    cv::rotate(this->_data->image, this->_data->image, cv::ROTATE_90_COUNTERCLOCKWISE);
    emit imageUpdated();
}

void ImageModel::editAutoWhiteBalance()
{
    // Edge cases
    if(!this->isImageLoaded())
        return;

    float percent = 2;
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

    emit imageUpdated();
}
