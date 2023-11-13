#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include "ImagePersistence.h"
#include <QObject>
#include <QPixmap>
#include <QImage>

#include <QDebug>

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
    // TODO:
    QPixmap getOriginalImageQPixmap(); // Save the edited

    // Histogram:
    QPixmap getHistogram(QSize histogramLabelSize);

    bool isImageLoaded();
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
    cv::Mat generateHistogramGridOverlay(cv::Mat source, int gridCols, int gridRows);
    cv::Mat generateHistogramRGB(cv::Mat source, cv::Mat img);


signals:
    void imageLoaded();
    void imageUpdated();
};

#endif // IMAGEMODEL_H
