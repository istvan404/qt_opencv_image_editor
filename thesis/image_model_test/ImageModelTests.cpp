#include "ImageModelTests.h"
//#include "ImagePersistenceMock.h"
#include "../image_editor/ImageModel.h"

ImageModelTests::ImageModelTests(QObject *parent) : QObject(parent)
{

}

void ImageModelTests::Test_Initialization_Empty()
{
    // SETUP
    ImageModel model;
    QSignalSpy loadedSignalSpy(&model, SIGNAL(imageLoaded()));

    // TEST
    QVERIFY(!model.isImageDataLoaded());
    QVERIFY(loadedSignalSpy.length() == 0);

    model.loadImage("");
    QVERIFY(!model.isImageDataLoaded());
    QVERIFY(loadedSignalSpy.length() == 0);
}

void ImageModelTests::Test_File_JPG()
{
    // SETUP
    ImageModel model;
    QSignalSpy loadedSignalSpy(&model, SIGNAL(imageLoaded()));
    QSignalSpy errorSignalSpy(&model, SIGNAL(imageLoadError()));

    // TEST
    QVERIFY(!model.isImageDataLoaded());
    QVERIFY(loadedSignalSpy.length() == 0);
    QVERIFY(errorSignalSpy.length() == 0);

    model.loadImage(path_jpg);
    QVERIFY(model.isImageDataLoaded());
    QVERIFY(loadedSignalSpy.length() == 1);
    QVERIFY(errorSignalSpy.length() == 0);
    QVERIFY(!model.isImageEdited());
}

void ImageModelTests::Test_File_PNG()
{
    // SETUP
    ImageModel model;
    QSignalSpy loadedSignalSpy(&model, SIGNAL(imageLoaded()));
    QSignalSpy errorSignalSpy(&model, SIGNAL(imageLoadError()));

    // TEST
    QVERIFY(!model.isImageDataLoaded());
    QVERIFY(loadedSignalSpy.length() == 0);
    QVERIFY(errorSignalSpy.length() == 0);

    model.loadImage(path_png);
    QVERIFY(model.isImageDataLoaded());
    QVERIFY(loadedSignalSpy.length() == 1);
    QVERIFY(errorSignalSpy.length() == 0);
    QVERIFY(!model.isImageEdited());
}

void ImageModelTests::Test_File_BMP()
{
    // SETUP
    ImageModel model;

    QSignalSpy loadedSignalSpy(&model, SIGNAL(imageLoaded()));
    QSignalSpy errorSignalSpy(&model, SIGNAL(imageLoadError()));

    // TEST
    QVERIFY(!model.isImageDataLoaded());
    QVERIFY(loadedSignalSpy.length() == 0);
    QVERIFY(errorSignalSpy.length() == 0);

    model.loadImage(path_bmp);
    QVERIFY(model.isImageDataLoaded());
    QVERIFY(loadedSignalSpy.length() == 1);
    QVERIFY(errorSignalSpy.length() == 0);
    QVERIFY(!model.isImageEdited());
}

void ImageModelTests::Test_Invalid_File_Path()
{
    // SETUP
    ImageModel model;
    QSignalSpy loadedSignalSpy(&model, SIGNAL(imageLoaded()));
    QSignalSpy errorSignalSpy(&model, SIGNAL(imageLoadError()));

    // TEST
    QVERIFY(!model.isImageDataLoaded());
    QVERIFY(loadedSignalSpy.length() == 0);
    QVERIFY(errorSignalSpy.length() == 0);

    model.loadImage("");
    QVERIFY(!model.isImageDataLoaded());
    QVERIFY(loadedSignalSpy.length() == 0);
    QVERIFY(errorSignalSpy.length() == 1);

    model.loadImage(path_invalid);
    QVERIFY(!model.isImageDataLoaded());
    QVERIFY(loadedSignalSpy.length() == 0);
    QVERIFY(errorSignalSpy.length() == 2);
}

void ImageModelTests::Test_Signals()
{
    // SETUP
    ImageModel model;
    QSignalSpy loadedSignalSpy(&model, SIGNAL(imageLoaded()));
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));
    QSignalSpy loadErrorSignalSpy(&model, SIGNAL(imageLoadError()));
    QSignalSpy saveErrorSignalSpy(&model, SIGNAL(imageSaveError()));

    // TEST
    QVERIFY(!model.isImageDataLoaded());

    QVERIFY(loadErrorSignalSpy.length() == 0);
    model.loadImage(path_invalid);
    QVERIFY(loadErrorSignalSpy.length() == 1);

    QVERIFY(loadedSignalSpy.length() == 0);
    model.loadImage(path_jpg);
    QVERIFY(loadedSignalSpy.length() == 1);

    QVERIFY(updatedSignalSpy.length() == 0);
    model.editFlipHorizontal();
    model.editFlipVertical();
    model.editRotate(90);
    model.editReset();
    QVERIFY(updatedSignalSpy.length() == 4);
    QVERIFY(loadedSignalSpy.length() == 1);
    QVERIFY(loadErrorSignalSpy.length() == 1);

    model.saveImage("");
    QVERIFY(saveErrorSignalSpy.length() == 1);
    model.saveImage("asd.txt");
    QVERIFY(saveErrorSignalSpy.length() == 2);
}

void ImageModelTests::Test_Nothing_Happens_Without_Image()
{
    // SETUP
    ImageModel model;
    QSignalSpy loadedSignalSpy(&model, SIGNAL(imageLoaded()));
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));
    QSignalSpy loadErrorSignalSpy(&model, SIGNAL(imageLoadError()));
    QSignalSpy saveErrorSignalSpy(&model, SIGNAL(imageSaveError()));

    // TEST
    QVERIFY(!model.isImageDataLoaded());

    model.editFlipHorizontal();
    model.editFlipVertical();
    model.editRotate(90);
    model.editRotate(-90);
    model.editRotate(180);
    model.editReset();
    model.editBrightness(10);
    model.editWhiteBalanceGW();
    model.editWhiteBalance(10);
    model.editShadows(10);
    model.editShadowsBasic(10);

    QVERIFY(!model.isImageDataLoaded());
    QVERIFY(loadedSignalSpy.length() == 0);
    QVERIFY(updatedSignalSpy.length() == 0);
    QVERIFY(loadErrorSignalSpy.length() == 0);
    QVERIFY(saveErrorSignalSpy.length() == 0);
}

void ImageModelTests::Test_Reset()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_jpg);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageDataLoaded());
    QVERIFY(!model.isImageEdited());

    model.editFlipHorizontal();
    model.editFlipVertical();
    QVERIFY(model.isImageEdited());

    model.editReset();
    QVERIFY(!model.isImageEdited());

    model.editRotate(90);
    QVERIFY(model.isImageEdited());

    model.editReset();
    QVERIFY(!model.isImageEdited());

    QVERIFY(updatedSignalSpy.length() == 5);
}

void ImageModelTests::Test_Flip_Vertical()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_jpg);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageDataLoaded());
    QVERIFY(!model.isImageEdited());

    model.editFlipVertical();
    QVERIFY(model.isImageEdited());

    model.editFlipVertical();
    QVERIFY(!model.isImageEdited());

    model.editFlipVertical();
    model.editFlipVertical();
    QVERIFY(!model.isImageEdited());

    QVERIFY(updatedSignalSpy.length() == 4);
}

void ImageModelTests::Test_Flip_Horizontal()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_jpg);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageDataLoaded());
    QVERIFY(!model.isImageEdited());

    model.editFlipHorizontal();
    QVERIFY(model.isImageEdited());

    model.editFlipHorizontal();
    QVERIFY(!model.isImageEdited());

    model.editFlipHorizontal();
    model.editFlipHorizontal();
    QVERIFY(!model.isImageEdited());

    QVERIFY(updatedSignalSpy.length() == 4);
}

void ImageModelTests::Test_Flip_Mixed()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_jpg);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageDataLoaded());
    QVERIFY(!model.isImageEdited());

    model.editFlipHorizontal();
    model.editFlipVertical();
    QVERIFY(model.isImageEdited());

    model.editFlipHorizontal();
    QVERIFY(model.isImageEdited());

    model.editFlipVertical();
    QVERIFY(!model.isImageEdited());

    model.editFlipHorizontal();
    model.editFlipVertical();
    model.editFlipHorizontal();
    model.editFlipVertical();
    QVERIFY(!model.isImageEdited());

    QVERIFY(updatedSignalSpy.length() == 8);
}

void ImageModelTests::Test_Rotate_90_CW()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_jpg);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(!model.isImageEdited());

    model.editRotate(90);
    QVERIFY(model.isImageEdited());

    model.editRotate(90);
    QVERIFY(model.isImageEdited());

    model.editRotate(90);
    model.editRotate(90);
    QVERIFY(!model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 4);
}

void ImageModelTests::Test_Rotate_90_CCW()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_jpg);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(!model.isImageEdited());

    model.editRotate(-90);
    QVERIFY(model.isImageEdited());

    model.editRotate(-90);
    QVERIFY(model.isImageEdited());

    model.editRotate(-90);
    model.editRotate(-90);
    QVERIFY(!model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 4);
}

void ImageModelTests::Test_Rotate_180()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_jpg);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(!model.isImageEdited());

    model.editRotate(180);
    QVERIFY(model.isImageEdited());

    model.editRotate(180);
    QVERIFY(!model.isImageEdited());

    model.editRotate(180);
    model.editRotate(180);
    QVERIFY(!model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 4);
}

void ImageModelTests::Test_Rotate_Mixed()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_jpg);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(!model.isImageEdited());

    model.editRotate(90);
    model.editRotate(-90);
    QVERIFY(!model.isImageEdited());

    model.editRotate(180);
    model.editRotate(90);
    model.editRotate(90);
    QVERIFY(!model.isImageEdited());

    model.editRotate(180);
    model.editRotate(-90);
    model.editRotate(-90);
    QVERIFY(!model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 8);
}

void ImageModelTests::Test_Rotate_Invalid_Values()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_jpg);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(!model.isImageEdited());

    model.editRotate(91);
    model.editRotate(-89);
    model.editRotate(181);
    model.editRotate(0);
    model.editRotate(-1);
    model.editRotate(1234567);
    model.editRotate(9999999);
    QVERIFY(!model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 0);
}

void ImageModelTests::Test_Brightness_Values()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_jpg);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageDataLoaded());
    QVERIFY(!model.isImageEdited());

    // TEST - INVALID
    model.editBrightness(1000000);
    model.editBrightness(-1000000);
    model.editBrightness(-51);
    model.editBrightness(51);
    QVERIFY(!model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - DEFAULT
    model.editBrightness(0);
    QVERIFY(!model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - VALID
    model.editBrightness(-50);
    model.editBrightness(50);
    model.editBrightness(10);
    QVERIFY(model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 3);
}

void ImageModelTests::Test_WhiteBalance_Values()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_jpg);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageDataLoaded());
    QVERIFY(!model.isImageEdited());

    // TEST - INVALID
    model.editWhiteBalance(-1);
    model.editWhiteBalance(-9999);
    model.editWhiteBalance(21);
    model.editWhiteBalance(9999);
    QVERIFY(!model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - DEFAULT
    model.editWhiteBalance(0);
    QVERIFY(!model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - VALID
    model.editWhiteBalance(1);
    model.editWhiteBalance(20);
    QVERIFY(model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 2);
}

void ImageModelTests::Test_WhiteBalance_GW()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_png);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageDataLoaded());
    QVERIFY(!model.isImageEdited());

    // TEST - Gray World
    model.editWhiteBalanceGW();
    QVERIFY(model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 1);
}

void ImageModelTests::Test_ShadowBasic_Values()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_jpg);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageDataLoaded());
    QVERIFY(!model.isImageEdited());

    // TEST - INVALID
    model.editShadowsBasic(-1);
    model.editShadowsBasic(51);
    model.editShadowsBasic(-9999);
    model.editShadowsBasic(9999);
    QVERIFY(!model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - DEFAULT
    model.editShadowsBasic(0);
    QVERIFY(!model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - VALID
    model.editShadowsBasic(1);
    model.editShadowsBasic(50);
    QVERIFY(model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 2);
}

void ImageModelTests::Test_Shadows_Values()
{
    // SETUP
    ImageModel model;
    model.loadImage(path_jpg);
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageDataLoaded());
    QVERIFY(!model.isImageEdited());

    // TEST - INVALID
    model.editShadows(-1);
    model.editShadows(51);
    model.editShadows(-9999);
    model.editShadows(9999);
    QVERIFY(!model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - DEFAULT
    model.editShadows(0);
    QVERIFY(!model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - VALID
    model.editShadows(1);
    model.editShadows(50);
    QVERIFY(model.isImageEdited());
    QVERIFY(updatedSignalSpy.length() == 2);
}

QTEST_MAIN(ImageModelTests)
