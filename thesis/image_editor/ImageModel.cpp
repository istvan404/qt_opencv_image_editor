#include "ImageModel.h"

ImageModel::ImageModel(/*ImagePersistenceInterface* persistence, */QObject *parent)
    : QObject{parent}
{
    //_persistence = persistence;
}

void ImageModel::loadImage(QString path)
{
    QSet<QString> extensions = {"jpg", "bmp", "png", "JPG", "PNG", "BMP", "jpeg", "JPEG"};
    QFileInfo file(path);

    if( !file.exists() || !file.isFile() )
    {
        emit imageLoadError();
        return;
    }

    QString extension = file.suffix();

    if( !extensions.contains(extension) )
    {
        emit imageLoadError();
        return;
    }

    //ImageData* data = _persistence->load(path);
    cv::Mat image = cv::imread(path.toStdString(), cv::IMREAD_COLOR);
    ImageData* data = new ImageData(path, image);

    if( data->Image.empty() )
    {
        emit imageLoadError();
        return;
    }

    if( data->Image.cols <= 0 || data->Image.rows <= 0)
    {
        emit imageLoadError();
        return;
    }

    if( data->Image.type() != CV_8UC3 ) // unsigned 8 bit with 3 channels
    {
        emit imageLoadError();
        return;
    }

    if( data->Image.channels() != 3 )
    {
        emit imageLoadError();
        return;
    }

    this->_data = data;
    emit imageLoaded();
}

void ImageModel::saveImage(QString path)
{
    if(!this->isImageDataLoaded())
    {
        emit imageSaveError();
        return;
    }

    if(this->isImageEmpty())
    {
        emit imageSaveError();
        return;
    }

    if( path.isEmpty() )
    {
        emit imageSaveError();
        return;
    }

    QSet<QString> extensions = {"jpg", "bmp", "png", "JPG", "PNG", "BMP", "jpeg", "JPEG"};
    QFileInfo file(path);
    QString extension = file.suffix();

    qDebug() << "Model: saving file. path: " << path;
    qDebug() << "Model: saving file. extension: " << extension;

    if( !extensions.contains(extension) )
    {
        emit imageSaveError();
        qDebug() << "Model: we don't support this extension!";
        return;
    }

    //_persistence->save(path, this->_data);
    cv::imwrite(path.toStdString(), _data->Image);
}

bool ImageModel::isImageDataLoaded()
{
    return this->_data != nullptr;
}

bool ImageModel::isImageEmpty()
{
    if( this->_data->Image.rows <= 0 || this->_data->Image.cols <= 0 )
    {
        return true;
    }

    return this->_data->Image.empty();
}

bool ImageModel::isImageEdited()
{
    if(!isImageDataLoaded())
    {
        return false;
    }

    if(isImageEmpty())
    {
        return false;
    }

    if(_data->Image.rows != _data->ImageOriginal.rows)
    {
        return true;
    }

    if(_data->Image.cols != _data->ImageOriginal.cols)
    {
        return true;
    }

    if(_data->Image.channels() != 3 || _data->ImageOriginal.channels() != 3)
    {
        return true;
    }

    for(int y = 0; y < this->_data->Image.rows; y++)
    {
        for(int x = 0; x < this->_data->Image.cols; x++)
        {
            if( _data->Image.at<cv::Vec3b>(y,x) != _data->ImageOriginal.at<cv::Vec3b>(y,x) )
            {
                return true;
            }
        }
    }

    return false;
}

QPixmap ImageModel::getEditedImageQPixmap()
{
    if(!isImageDataLoaded())
    {
        return QPixmap();
    }

    if(isImageEmpty())
    {
        return QPixmap();
    }

    cv::Mat img = this->_data->Image;

    QImage qimg(img.data,
                img.cols,
                img.rows,
                static_cast<int>(img.step),
                QImage::Format_RGB888);

    return QPixmap::fromImage(qimg.rgbSwapped());
}

QPixmap ImageModel::getOriginalImageQPixmap()
{
    if(!isImageDataLoaded())
    {
        return QPixmap();
    }

    if(isImageEmpty())
    {
        return QPixmap();
    }

    cv::Mat img = this->_data->ImageOriginal;

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
    if(!this->isImageDataLoaded())
    {
        return QPixmap();           // TODO: Revisit this solution to edge case, maybe EMIT some problem? THROW error?
    }

    double aspectRatio = histogramLabelSize.width() / histogramLabelSize.height();
    int width = 256;
    int height = 256 / aspectRatio;

    cv::Mat img = this->_data->Image;
    cv::Mat histogram(height, width, CV_8UC3, cv::Scalar(10,10,10));

    generateHistogramRGB(histogram, img, histogram);

    if(histogram.rows <= 0 || histogram.cols <= 0)
    {
        return QPixmap();
    }

    generateHistogramGridOverlay(histogram, 3, 2, histogram);

    QImage qimg(histogram.data,
                histogram.cols,
                histogram.rows,
                static_cast<int>(histogram.step),
                QImage::Format_RGB888);

    return QPixmap::fromImage(qimg.rgbSwapped());
}

void ImageModel::generateHistogramRGB(cv::Mat source, cv::Mat image, cv::Mat output) const
{
    if(image.rows <= 0 || image.cols <= 0)
    {
        return;
    }

    if(image.channels() != 3)
    {
        return;
    }

    if(source.rows <= 0 || source.cols <= 0)
    {
        return;
    }

    if(source.channels() != 3)
    {
        return;
    }

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
        float a = all_values[i-1];
        float b = all_values[i];
        if( (a/b) > 1.5 )
        {
            ceiling = b;
        }
    }

    if(ceiling != -1)
    {
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

    output = histogram.clone();
}

void ImageModel::generateHistogramGridOverlay(cv::Mat source, int gridCols, int gridRows, cv::Mat output) const
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

    output = img.clone();
}

void ImageModel::editReset()
{
    if(!this->isImageDataLoaded())
    {
        return;
    }

    if(this->isImageEmpty())
    {
        return;
    }

    this->_data->Image = this->_data->ImageOriginal.clone();
    emit imageUpdated();
}

void ImageModel::editFlipHorizontal()
{
    if(!this->isImageDataLoaded())
    {
        return;
    }

    if(this->isImageEmpty())
    {
        return;
    }

    cv::flip(this->_data->Image, this->_data->Image, 0);
    emit imageUpdated();
}

void ImageModel::editFlipVertical()
{
    if(!this->isImageDataLoaded())
    {
        return;
    }

    if(this->isImageEmpty())
    {
        return;
    }

    cv::flip(this->_data->Image, this->_data->Image, 1);
    emit imageUpdated();
}

void ImageModel::editRotate(int degree)
{
    if(!this->isImageDataLoaded())
    {
        return;
    }

    if(this->isImageEmpty())
    {
        return;
    }

    if(degree == 90)
    {
        cv::rotate(this->_data->Image, this->_data->Image, cv::ROTATE_90_CLOCKWISE);
    }
    else if(degree == -90)
    {
        cv::rotate(this->_data->Image, this->_data->Image, cv::ROTATE_90_COUNTERCLOCKWISE);
    }
    else if(degree == 180)
    {
        cv::rotate(this->_data->Image, this->_data->Image, cv::ROTATE_180);
    }
    else
    {
        return;
    }

    emit imageUpdated();
}

void ImageModel::editWhiteBalance(int value)
{
    const float percent = value;
    const float percentLimitMin = 0;
    const float percentLimitMax = 20;
    const float halfPercent = percent / 200.0f;

    if(!this->isImageDataLoaded())
    {
        return;
    }

    if(this->isImageEmpty())
    {
        return;
    }

    if(percent <= percentLimitMin || percent > percentLimitMax)
    {
        return;
    }

    std::vector<cv::Mat> bgrChannelSplit;
    cv::split(this->_data->Image,bgrChannelSplit);
    for(int i=0;i<3;i++)
    {
        cv::Mat singleChannel;
        bgrChannelSplit[i].reshape(1,1).copyTo(singleChannel);
        cv::sort(singleChannel,singleChannel,cv::SORT_EVERY_ROW + cv::SORT_ASCENDING);
        //int floorValue = singleChannel.at<uchar>(cvFloor(((float)singleChannel.cols) * halfPercent));
        int ceilingValue = singleChannel.at<uchar>(cvCeil(((float)singleChannel.cols) * (1.0 - halfPercent)));

        //bgrChannelSplit[i].setTo(floorValue,bgrChannelSplit[i] < floorValue);
        bgrChannelSplit[i].setTo(ceilingValue,bgrChannelSplit[i] > ceilingValue);

        cv::normalize(bgrChannelSplit[i],bgrChannelSplit[i],0,255,cv::NORM_MINMAX);
    }
    cv::merge(bgrChannelSplit,this->_data->Image);

    emit imageUpdated();
}

void ImageModel::editBrightness(int value)
{
    int valueLimitMin = -50;
    int valueLimitMax = 50;
    int valueDefault = 0;

    if(!this->isImageDataLoaded())
    {
        return;
    }

    if(this->isImageEmpty())
    {
        return;
    }

    if(value < valueLimitMin)
    {
        return;
    }

    if(value > valueLimitMax)
    {
        return;
    }

    if(value == valueDefault)
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

void ImageModel::editWhiteBalanceGW()
{
    // Gray World Algorithm
    if(!this->isImageDataLoaded())
    {
        return;
    }

    if(this->isImageEmpty())
    {
        return;
    }

    const int rows = this->_data->Image.rows;
    const int cols = this->_data->Image.cols;

    int Bsum = 0;
    int Gsum = 0;
    int Rsum = 0;

    for(int y = 0; y < rows; y++)
    {
        for(int x = 0; x < cols; x++)
        {
            Bsum += this->_data->Image.at<cv::Vec3b>(y,x)[0];
            Gsum += this->_data->Image.at<cv::Vec3b>(y,x)[1];
            Rsum += this->_data->Image.at<cv::Vec3b>(y,x)[2];
        }
    }

    const int Bavg = Bsum / (rows*cols);
    const int Gavg = Gsum / (rows*cols);
    const int Ravg = Rsum / (rows*cols);

    if(Bavg == Gavg && Gavg == Ravg)
    {
        // The image already satisfies the gray world assumption
        return;
    }

    if(Bavg <= 0 || Ravg <= 0)
    {
        // Avoiding the divison with zero
        return;
    }

    const double beta  = (double)Gavg / (double)Bavg;
    const double alpha = (double)Gavg / (double)Ravg;

    for(int y = 0; y < rows; y++)
    {
        for(int x = 0; x < cols; x++)
        {
            this->_data->Image.at<cv::Vec3b>(y,x)[0] = cv::saturate_cast<uchar>(this->_data->Image.at<cv::Vec3b>(y,x)[0] * beta);
            // We do not need to modify the green channel.
            this->_data->Image.at<cv::Vec3b>(y,x)[2] = cv::saturate_cast<uchar>(this->_data->Image.at<cv::Vec3b>(y,x)[2] * alpha);
        }
    }

    emit imageUpdated();
}

void ImageModel::editShadowsBasic(int value)
{
    const int valueLimitMin = 0;
    const int valueLimitMax = 50;
    const int valueDefault = 0;

    if(!this->isImageDataLoaded())
    {
        return;
    }

    if(this->isImageEmpty())
    {
        return;
    }

    if(value < valueLimitMin)
    {
        return;
    }

    if(value > valueLimitMax)
    {
        return;
    }

    if(value == valueDefault)
    {
        return;
    }

    const int cutoff = 100;    // Cut-off value for shadow/dark pixels

    for(int y = 0; y < this->_data->Image.rows; y++)
    {
        for(int x = 0; x < this->_data->Image.cols; x++)
        {
            for(int c = 0; c < this->_data->Image.channels(); c++)
            {
                if( this->_data->Image.at<cv::Vec3b>(y,x)[c] < cutoff )
                {
                    this->_data->Image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(this->_data->Image.at<cv::Vec3b>(y,x)[c] + value );
                }
            }
        }
    }

    emit imageUpdated();
}

void ImageModel::editShadows(int value)
{
    const int valueLimitMin = 0;
    const int valueLimitMax = 50;
    const int valueDefault = 0;

    if(!this->isImageDataLoaded())
    {
        return;
    }

    if(this->isImageEmpty())
    {
        return;
    }

    if(value < valueLimitMin)
    {
        return;
    }

    if(value > valueLimitMax)
    {
        return;
    }

    if(value == valueDefault)
    {
        return;
    }

    const int rows = this->_data->Image.rows;
    const int cols = this->_data->Image.cols;
    const int cutoff = 100;
    const double percentage = value / 200.0;

    cv::Mat mask = this->_data->Image.clone();
    cv::cvtColor(mask, mask, cv::COLOR_BGR2GRAY, 1);
    mask.setTo(255,mask > cutoff);

    cv::imwrite("C:/Users/Admin/Desktop/mask.jpg", mask);

    for(int y = 0; y < rows; y++)
    {
        for(int x = 0; x < cols; x++)
        {
            mask.at<uchar>(y,x) = (255 - mask.at<uchar>(y,x)) * percentage;
        }
    }

    cv::imwrite("C:/Users/Admin/Desktop/mask_inverted.jpg", mask);

    for(int y = 0; y < rows; y++)
    {
        for(int x = 0; x < cols; x++)
        {
            for(int c = 0; c < this->_data->Image.channels(); c++)
            {
                this->_data->Image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(this->_data->Image.at<cv::Vec3b>(y,x)[c] + (mask.at<uchar>(y,x)));
            }
        }
    }

    emit imageUpdated();
}
