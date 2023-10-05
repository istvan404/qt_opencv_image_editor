#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include <QObject>
#include "ImagePersistence.h"

class ImageModel : public QObject
{
    Q_OBJECT
public:
    explicit ImageModel(QObject *parent = nullptr);

signals:

};

#endif // IMAGEMODEL_H
