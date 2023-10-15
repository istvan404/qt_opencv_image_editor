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
    QPixmap getEditedRedHistogram(QSize imageLabelSize);
    QPixmap getEditedGreenHistogram(QSize imageLabelSize);
    QPixmap getEditedBlueHistogram(QSize imageLabelSize);

    void editFlipHorizontal();
    void editFlipVertical();
    void editRotate90Plus();
    void editRotate90Minus();
    void editToggleImageScale(bool toggle);

private:
    ImagePersistenceInterface* _persistence;
    ImageData _data;
    bool _toggleImageFillSpace;

    cv::Mat resizeMatrix(cv::Mat input, QSize availableSize);
    cv::Mat resizeMatrixBySteps(cv::Mat input, QSize targetSize, cv::InterpolationFlags interpolation, int steps);

signals:
    void imageLoaded();
};

#endif // IMAGEMODEL_H
