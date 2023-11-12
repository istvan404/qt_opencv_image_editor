#include "ImageModel.h"

ImageModel::ImageModel(ImagePersistenceInterface* persistence, QObject *parent)
    : QObject{parent}
{
    _persistence = persistence;
}

void ImageModel::loadImage(QString path)
{
    this->_data = _persistence->load(path);
    qDebug() << "img.type() == CV_8UC4 ->" << (this->_data->Image.type() == CV_8UC4); // unsigned 8 bit with 4 channels
    qDebug() << "img.type() == CV_8UC3 ->" << (this->_data->Image.type() == CV_8UC3); // unsigned 8 bit with 3 channels
    qDebug() << "img.type() == CV_8UC1 ->" << (this->_data->Image.type() == CV_8UC1); // unsigned 8 bit with 1 channels
    emit imageLoaded();
}

void ImageModel::saveImage(QString path)
{
    if(!this->isImageLoaded())
    {
        return;
    }

    _persistence->save(path, this->_data);
}

bool ImageModel::isImageLoaded()
{
    return this->_data != nullptr;
}

QPixmap ImageModel::getEditedImageQPixmap()
{
    cv::Mat img = this->_data->Image;

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
    {
        return QPixmap();           // TODO: Revisit this solution to edge case, maybe EMIT some problem? THROW error?
    }

    /*int width = histogramLabelSize.width();
    if(width % 2 != 0)
        width++;
    int height = histogramLabelSize.height();
    if(height % 2 != 0)
        height++;*/

    double aspectRatio = histogramLabelSize.width() / histogramLabelSize.height();
    int width = 256;
    int height = 256 / aspectRatio;

    cv::Mat img = this->_data->Image;
    cv::Mat histogram(height, width, CV_8UC3, cv::Scalar(10,10,10));

    histogram = generateHistogramRGB(histogram, img);

    histogram = generateHistogramGridOverlay(histogram, 3, 2);

    QImage qimg(histogram.data,
                histogram.cols,
                histogram.rows,
                static_cast<int>(histogram.step),
                QImage::Format_RGB888);

    return QPixmap::fromImage(qimg.rgbSwapped());
}

cv::Mat ImageModel::generateHistogramRGB(cv::Mat source, cv::Mat image)
{
    cv::Mat histogram = source;
    cv::Mat img = image;
    std::vector<cv::Mat> bgr_channels;
    int histSize = 256;
    float range[] = {0,256};
    const float* histRange[] = {range};
    bool uniform = true;
    bool accumulate = false;
    cv::Mat b_channel;
    cv::Mat g_channel;
    cv::Mat r_channel;

    cv::split(img, bgr_channels);

    cv::calcHist(&bgr_channels[0],
            1,
            0,
            cv::Mat(),
            b_channel,
            1,
            &histSize,
            histRange,
            uniform,
            accumulate);

    cv::calcHist(&bgr_channels[1],
            1,
            0,
            cv::Mat(),
            g_channel,
            1,
            &histSize,
            histRange,
            uniform,
            accumulate);

    cv::calcHist(&bgr_channels[2],
            1,
            0,
            cv::Mat(),
            r_channel,
            1,
            &histSize,
            histRange,
            uniform,
            accumulate);

    int hist_h = histogram.rows;
/*
    for( int i = 0; i < histSize; i++ )
    {
        qDebug() << "b_channel[" << i << "] = " << cvRound(b_channel.at<float>(i))
                 << " | "
                 << "g_channel[" << i << "] = " << cvRound(g_channel.at<float>(i))
                 << " | "
                 << "r_channel[" << i << "] = " << cvRound(r_channel.at<float>(i));
    }

    qDebug() << "hist_h = " << hist_h;*/

    float max = 0;
    for(int i = 0; i < histSize; i++)
    {
        if( b_channel.at<float>(i) > max )
            max = b_channel.at<float>(i);
        if( g_channel.at<float>(i) > max )
            max = g_channel.at<float>(i);
        if( r_channel.at<float>(i) > max )
            max = r_channel.at<float>(i);
    }

    std::vector<float> all_values(histSize*3);
    for(int i = 0; i < histSize; i++)
    {
        all_values[i] = b_channel.at<float>(i);
        all_values[i+histSize] = b_channel.at<float>(i);
        all_values[i+histSize+histSize] = b_channel.at<float>(i);
    }

    std::sort(all_values.begin(), all_values.end());
    std::reverse(all_values.begin(), all_values.end());

    float ceiling = -1;
    for(int i = 1; i < (3*histSize)*0.2; i++)
    {
        //qDebug() << "all_values[" << i << "] =" << all_values[i];
        float a = all_values[i-1];
        float b = all_values[i];
        if( (a/b) > 1.5 )
        {
            ceiling = b;
        }
    }

    if(ceiling != -1)
    {
        //qDebug() << "ceiling := " << ceiling;
        b_channel.setTo(ceiling,b_channel > ceiling);
        g_channel.setTo(ceiling,g_channel > ceiling);
        r_channel.setTo(ceiling,r_channel > ceiling);
    }

    normalize(b_channel, b_channel, 0, hist_h, cv::NORM_MINMAX, -1, cv::Mat() );
    normalize(g_channel, g_channel, 0, hist_h, cv::NORM_MINMAX, -1, cv::Mat() );
    normalize(r_channel, r_channel, 0, hist_h, cv::NORM_MINMAX, -1, cv::Mat() );

    cv::Scalar color_b = cv::Scalar(255, 0, 0);         // blue
    cv::Scalar color_g = cv::Scalar(0, 255, 0);         // green
    cv::Scalar color_r = cv::Scalar(0, 0, 255);         // red
    cv::Scalar color_gr = cv::Scalar(0, 255, 255);      // yellow
    cv::Scalar color_br = cv::Scalar(255, 0, 255);      // magenta
    cv::Scalar color_bg = cv::Scalar(255, 255, 0);      // cyan
    cv::Scalar color_bgr = cv::Scalar(255, 255, 255);   // white

    for( int i = 0; i < histSize; i++ )
    {
        /*qDebug() << "b_channel[" << i << "] = " << cvRound(b_channel.at<float>(i))
                 << " | "
                 << "g_channel[" << i << "] = " << cvRound(g_channel.at<float>(i))
                 << " | "
                 << "r_channel[" << i << "] = " << cvRound(r_channel.at<float>(i));*/

        int b_start = hist_h - cvRound(b_channel.at<float>(i));
        int g_start = hist_h - cvRound(g_channel.at<float>(i));
        int r_start = hist_h - cvRound(r_channel.at<float>(i));

        if(b_start < g_start && b_start < r_start)
        {
            // blue is the highest
            cv::line(histogram, cv::Point(i, b_start), cv::Point(i, hist_h), color_b, 1, 8, 0);
            if( g_start < r_start )
            {
                // green is the second highest
                cv::line(histogram, cv::Point(i, g_start), cv::Point(i, hist_h), color_bg, 1, 8, 0);
                cv::line(histogram, cv::Point(i, r_start), cv::Point(i, hist_h), color_bgr, 1, 8, 0);
            }
            else
            {
                // red is the second highest
                cv::line(histogram, cv::Point(i, r_start), cv::Point(i, hist_h), color_br, 1, 8, 0);
                cv::line(histogram, cv::Point(i, g_start), cv::Point(i, hist_h), color_bgr, 1, 8, 0);
            }
        }
        else if(g_start < b_start && g_start < r_start)
        {
            // green is the highest
            cv::line(histogram, cv::Point(i, g_start), cv::Point(i, hist_h), color_g, 1, 8, 0);
            if(b_start < r_start)
            {
                // blue is the second highest
                cv::line(histogram, cv::Point(i, b_start), cv::Point(i, hist_h), color_bg, 1, 8, 0);
                cv::line(histogram, cv::Point(i, r_start), cv::Point(i, hist_h), color_bgr, 1, 8, 0);
            }
            else
            {
                // red is the second highest
                cv::line(histogram, cv::Point(i, r_start), cv::Point(i, hist_h), color_gr, 1, 8, 0);
                cv::line(histogram, cv::Point(i, b_start), cv::Point(i, hist_h), color_bgr, 1, 8, 0);
            }
        }
        else
        {
            // red is the highest or some are equal
            cv::line(histogram, cv::Point(i, r_start), cv::Point(i, hist_h), color_r, 1, 8, 0);
            if(b_start < g_start)
            {
                // blue is the second highest
                cv::line(histogram, cv::Point(i, b_start), cv::Point(i, hist_h), color_br, 1, 8, 0);
                cv::line(histogram, cv::Point(i, g_start), cv::Point(i, hist_h), color_bgr, 1, 8, 0);
            }
            else
            {
                // green is the second highest
                cv::line(histogram, cv::Point(i, g_start), cv::Point(i, hist_h), color_gr, 1, 8, 0);
                cv::line(histogram, cv::Point(i, b_start), cv::Point(i, hist_h), color_bgr, 1, 8, 0);
            }
        }
    }

    return histogram;
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

void ImageModel::editReset()
{
    if(!this->isImageLoaded())
    {
        return;
    }

    this->_data->Image = this->_data->ImageOriginal.clone();
    emit imageUpdated();
}

void ImageModel::editFlipHorizontal()
{
    if(!this->isImageLoaded())
    {
        return;
    }

    cv::flip(this->_data->Image, this->_data->Image, 0);
    emit imageUpdated();
}

void ImageModel::editFlipVertical()
{
    if(!this->isImageLoaded())
    {
        return;
    }

    cv::flip(this->_data->Image, this->_data->Image, 1);
    emit imageUpdated();
}

void ImageModel::editRotate(int degree)
{
    if(!this->isImageLoaded())
    {
        return;
    }

    if(degree == 90)
        cv::rotate(this->_data->Image, this->_data->Image, cv::ROTATE_90_CLOCKWISE);
    else if(degree == -90)
        cv::rotate(this->_data->Image, this->_data->Image, cv::ROTATE_90_COUNTERCLOCKWISE);
    else if(degree == 180)
        cv::rotate(this->_data->Image, this->_data->Image, cv::ROTATE_180);

    emit imageUpdated();
}

void ImageModel::editAutoWhiteBalance(int value)
{
    float percent = value;
    const float percentLimitMin = 0;
    const float percentLimitMax = 20;

    if(!this->isImageLoaded())
    {
        return;
    }

    if(this->_data->Image.channels() != 3)
    {
        return;
    }

    if(percent <= percentLimitMin)
        return;

    if(percent > percentLimitMax)
        return;

    float halfPercent = percent / 200.0f;

    std::vector<cv::Mat> bgrChannelSplit;
    cv::split(this->_data->Image,bgrChannelSplit);
    for(int i=0;i<3;i++) {

        cv::Mat singleChannel;
        bgrChannelSplit[i].reshape(1,1).copyTo(singleChannel);
        cv::sort(singleChannel,singleChannel,cv::SORT_EVERY_ROW + cv::SORT_ASCENDING);
        int floorValue = singleChannel.at<uchar>(cvFloor(((float)singleChannel.cols) * halfPercent));
        int ceilingValue = singleChannel.at<uchar>(cvCeil(((float)singleChannel.cols) * (1.0 - halfPercent)));

        bgrChannelSplit[i].setTo(floorValue,bgrChannelSplit[i] < floorValue);
        bgrChannelSplit[i].setTo(ceilingValue,bgrChannelSplit[i] > ceilingValue);

        cv::normalize(bgrChannelSplit[i],bgrChannelSplit[i],0,255,cv::NORM_MINMAX);
    }
    cv::merge(bgrChannelSplit,this->_data->Image);

    emit imageUpdated();
}

void ImageModel::editBrightness(int value)
{
    int valueLimitMin = 0;
    int valueLimitMax = 50;

    if(!this->isImageLoaded())
    {
        return;
    }

    if(this->_data->Image.channels() != 3)
    {
        return;
    }

    if(value <= valueLimitMin)
    {
        return;
    }

    if(value > valueLimitMax)
    {
        return;
    }

    double alpha = 1;   // Contrast control
    int beta = value;   // Brightness control

    for(int y = 0; y < this->_data->Image.rows; y++)
    {
        for(int x = 0; x < this->_data->Image.cols; x++)
        {
            for(int c = 0; c < this->_data->Image.channels(); c++)
            {
                // This is an option for brightening the shadows and darks
                /*if( this->_data->image.at<cv::Vec3b>(y,x)[c] > 100 )
                    continue;*/
                this->_data->Image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>( alpha * this->_data->Image.at<cv::Vec3b>(y,x)[c] + beta );
            }
        }
    }

    emit imageUpdated();
}
