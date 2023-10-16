#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QObject>
#include <QPixmap>
#include <QImage>
#include "ImagePersistence.h"

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

    QPixmap getEditedImageQPixmap(QSize imageLabelSize);
    // TODO:
    QPixmap getOriginalImageQPixmap(QSize imageLabelSize); // Save the edited
    QPixmap getHistogram(QSize histogramLabelSize);

    bool isImageLoaded();
    void editFlipHorizontal();
    void editFlipVertical();
    void editRotate90Plus();
    void editRotate90Minus();
    void editToggleImageScale(bool toggle);

private:
    ImagePersistenceInterface* _persistence = nullptr;
    ImageData* _data = nullptr;
    bool _toggleImageFillSpace;

    cv::Mat resizeMatrix(cv::Mat input, QSize availableSize);
    cv::Mat resizeMatrixBySteps(cv::Mat input, QSize targetSize, cv::InterpolationFlags interpolation, int steps);

signals:
    void imageLoaded();
};

#endif // IMAGEMODEL_H
