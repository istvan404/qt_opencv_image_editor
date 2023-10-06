#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QObject>
#include <QPixmap>
#include <QImage>
#include "ImagePersistence.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class ImageModel : public QObject
{
    Q_OBJECT
public:
    explicit ImageModel(ImagePersistenceInterface* persistence, QObject *parent = nullptr);

    bool loadImage(QString path);
    QPixmap getQPixmap();

private:
    ImagePersistenceInterface* _persistence;
    ImageData _data;

signals:
    void imageLoaded();
};

#endif // IMAGEMODEL_H
