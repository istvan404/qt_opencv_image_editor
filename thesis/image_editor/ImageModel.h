#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include "ImagePersistence.h"
#include <QObject>
#include <QString>
#include <QFileInfo>

#include <QPixmap>
#include <QImage>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/hal/interface.h>

class ImageModel : public QObject
{
    Q_OBJECT
public:
    explicit ImageModel(ImagePersistenceInterface* persistence, QObject *parent = nullptr);

    void loadImage(QString path);
    void saveImage(QString path);

    QPixmap getEditedImageQPixmap();
    QPixmap getOriginalImageQPixmap();

    // Histogram:
    QPixmap getHistogram(QSize histogramLabelSize);

    bool isImageDataLoaded();
    bool isImageEmpty();
    void editReset();
    void editFlipHorizontal();
    void editFlipVertical();
    void editRotate(int degree);
    void editWhiteBalance(int value);
    void editWhiteBalanceGW();
    void editBrightness(int value);
    void editShadowsBasic(int value);
    void editShadows(int value);

private:
    ImagePersistenceInterface* _persistence = nullptr;
    ImageData* _data = nullptr;

    // Histogram:
    void generateHistogramRGB(cv::Mat source, cv::Mat img, cv::Mat output);
    void generateHistogramGridOverlay(cv::Mat source, int gridCols, int gridRows, cv::Mat output);

signals:
    void imageLoaded();
    void imageUpdated();
    void imageLoadError();
};

#endif // IMAGEMODEL_H
