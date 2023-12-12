#include "ImageModelTests.h"
#include "ImagePersistenceMock.h"
#include "../image_editor/ImageModel.h"

ImageModelTests::ImageModelTests(QObject *parent) : QObject(parent)
{

}

void ImageModelTests::Test_Initialization()
{
    // SETUP
    ImageData emptySave;
    ImagePersistenceMock mock(emptySave);
    ImageModel model(&mock);

    // TEST
    QVERIFY(!model.isImageLoaded());
}

QTEST_MAIN(ImageModelTests)
