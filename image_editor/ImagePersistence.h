#ifndef IMAGEPERSISTENCE_H
#define IMAGEPERSISTENCE_H

#include "ImageSave.h"
#include <QString>

class ImagePersistenceInterface
{
public:
    virtual void save(QString path, ImageSave state) = 0;
    virtual ImageSave load(QString path) = 0;
};

class ImagePersistence : public ImagePersistenceInterface
{
public:
    ImagePersistence();
    void save(QString path, ImageSave state);
    ImageSave load(QString path);
};

#endif // IMAGEPERSISTENCE_H
