#ifndef IMAGEPERSISTENCE_H
#define IMAGEPERSISTENCE_H

#include "ImageData.h"
#include <QString>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class ImagePersistenceInterface
{
public:
    virtual void save(QString path, ImageData state) = 0;
    virtual ImageData load(QString path) = 0;
};

class ImagePersistence : public ImagePersistenceInterface
{
public:
    ImagePersistence();
    void save(QString path, ImageData state);
    ImageData load(QString path);
};

#endif // IMAGEPERSISTENCE_H
