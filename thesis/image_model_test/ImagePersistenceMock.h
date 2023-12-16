#ifndef IMAGEPERSISTENCEMOCK_H
#define IMAGEPERSISTENCEMOCK_H

#include "../image_editor/ImagePersistence.h"

class ImagePersistenceMock : public ImagePersistenceInterface
{
public:
    ImagePersistenceMock(ImageData state);
    void save(QString path, ImageData* state);
    ImageData* load(QString path);
private:
    ImageData _save;
};

#endif // IMAGEPERSISTENCEMOCK_H
