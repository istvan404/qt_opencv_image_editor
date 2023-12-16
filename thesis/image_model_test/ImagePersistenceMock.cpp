#include "ImagePersistenceMock.h"

ImagePersistenceMock::ImagePersistenceMock(ImageData save): _save(save)
{
}

void ImagePersistenceMock::save(QString path, ImageData* state)
{
}

ImageData* ImagePersistenceMock::load(QString path)
{
    return &_save;
    // TODO: Change interface pointer to non pointer!!
}
