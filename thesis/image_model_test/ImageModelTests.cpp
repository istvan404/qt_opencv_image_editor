#include "ImageModelTests.h"
#include "ImagePersistenceMock.h"
#include "../image_editor/ImageModel.h"

ImageModelTests::ImageModelTests(QObject *parent) : QObject(parent)
{

}

void ImageModelTests::Test_Initialization_Empty()
{
    // SETUP
    ImageData emptySave;
    ImagePersistenceMock mock(emptySave);
    ImageModel model(&mock);

    QSignalSpy loadedSignalSpy(&model, SIGNAL(imageLoaded()));

    // TEST
    QVERIFY(emptySave.Image.empty());
    QVERIFY(emptySave.ImageOriginal.empty());
    QVERIFY(!model.isImageLoaded());
    QVERIFY(loadedSignalSpy.length() == 0);

    model.loadImage("");
    QVERIFY(!model.isImageLoaded());
    QVERIFY(loadedSignalSpy.length() == 0);
}

void ImageModelTests::Test_Initialization_TestFile()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);

    // TEST
    QVERIFY(!model.isImageLoaded());
    model.loadImage("");
    QVERIFY(model.isImageLoaded());
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getEditedImageQPixmap().toImage());
    QVERIFY(model.getOriginalImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
}

void ImageModelTests::Test_Signals()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);

    QSignalSpy loadedSignalSpy(&model, SIGNAL(imageLoaded()));
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(!model.isImageLoaded());

    QVERIFY(loadedSignalSpy.length() == 0);
    model.loadImage("");
    QVERIFY(loadedSignalSpy.length() == 1);

    QVERIFY(updatedSignalSpy.length() == 0);
    model.editFlipHorizontal();
    model.editFlipVertical();
    model.editRotate(90);
    model.editReset();
    QVERIFY(updatedSignalSpy.length() == 4);
}

void ImageModelTests::Test_Flip_Vertical()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);
    model.loadImage("");

    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageLoaded());
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editFlipVertical();
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());

    model.editFlipVertical();
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editFlipVertical();
    model.editFlipVertical();
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    QVERIFY(updatedSignalSpy.length() == 4);
}

void ImageModelTests::Test_Flip_Horizontal()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);
    model.loadImage("");

    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageLoaded());
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editFlipHorizontal();
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());

    model.editFlipHorizontal();
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editFlipHorizontal();
    model.editFlipHorizontal();
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    QVERIFY(updatedSignalSpy.length() == 4);
}

void ImageModelTests::Test_Flip_Mixed()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);
    model.loadImage("");

    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageLoaded());
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editFlipHorizontal();
    model.editFlipVertical();
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());

    model.editFlipHorizontal();
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());

    model.editFlipVertical();
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editFlipHorizontal();
    model.editFlipVertical();
    model.editFlipHorizontal();
    model.editFlipVertical();
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    QVERIFY(updatedSignalSpy.length() == 8);
}

void ImageModelTests::Test_Rotate_90_CW()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);
    model.loadImage("");

    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editRotate(90);
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());

    model.editRotate(90);
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());

    model.editRotate(90);
    model.editRotate(90);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 4);
}

void ImageModelTests::Test_Rotate_90_CCW()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);
    model.loadImage("");

    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editRotate(-90);
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());

    model.editRotate(-90);
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());

    model.editRotate(-90);
    model.editRotate(-90);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 4);
}

void ImageModelTests::Test_Rotate_180()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);
    model.loadImage("");

    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editRotate(180);
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());

    model.editRotate(180);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editRotate(180);
    model.editRotate(180);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 4);
}

void ImageModelTests::Test_Rotate_Mixed()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);
    model.loadImage("");

    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editRotate(90);
    model.editRotate(-90);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editRotate(180);
    model.editRotate(90);
    model.editRotate(90);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editRotate(180);
    model.editRotate(-90);
    model.editRotate(-90);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 8);
}

void ImageModelTests::Test_Rotate_Invalid_Values()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);
    model.loadImage("");

    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editRotate(91);
    model.editRotate(-89);
    model.editRotate(181);
    model.editRotate(0);
    model.editRotate(-1);
    model.editRotate(1234567);
    model.editRotate(9999999);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 0);
}

void ImageModelTests::Test_Nothing_Happens_Without_Image()
{
    // SETUP
    ImageData emptySave;
    ImagePersistenceMock mock(emptySave);
    ImageModel model(&mock);

    QSignalSpy loadedSignalSpy(&model, SIGNAL(imageLoaded()));
    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(!model.isImageLoaded());

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

    QVERIFY(!model.isImageLoaded());
    QVERIFY(loadedSignalSpy.length() == 0);
    QVERIFY(updatedSignalSpy.length() == 0);
}

void ImageModelTests::Test_Reset()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);
    model.loadImage("");

    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageLoaded());
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editFlipHorizontal();
    model.editFlipVertical();
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());

    model.editReset();
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    model.editRotate(90);
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());

    model.editReset();
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    QVERIFY(updatedSignalSpy.length() == 5);
}

void ImageModelTests::Test_Brightness_Values()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);
    model.loadImage("");

    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageLoaded());
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    // TEST - INVALID
    model.editBrightness(1000000);
    model.editBrightness(-1000000);
    model.editBrightness(-51);
    model.editBrightness(51);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - DEFAULT
    model.editBrightness(0);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - VALID
    model.editBrightness(-50);
    model.editBrightness(50);
    model.editBrightness(10);
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 3);
}

void ImageModelTests::Test_WhiteBalance_Values()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);
    model.loadImage("");

    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageLoaded());
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    // TEST - INVALID
    model.editWhiteBalance(-1);
    model.editWhiteBalance(-9999);
    model.editWhiteBalance(21);
    model.editWhiteBalance(9999);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - DEFAULT
    model.editWhiteBalance(0);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - VALID
    model.editWhiteBalance(1);
    model.editWhiteBalance(20);
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 2);
}

void ImageModelTests::Test_ShadowBasic_Values()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);
    model.loadImage("");

    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageLoaded());
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    // TEST - INVALID
    model.editShadowsBasic(-1);
    model.editShadowsBasic(51);
    model.editShadowsBasic(-9999);
    model.editShadowsBasic(9999);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - DEFAULT
    model.editShadowsBasic(0);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - VALID
    model.editShadowsBasic(1);
    model.editShadowsBasic(50);
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 2);
}

void ImageModelTests::Test_Shadows_Values()
{
    // SETUP
    cv::Mat img = cv::imread(test_file_path.toStdString());
    ImageData save(test_file_path, img);
    ImagePersistenceMock mock(save);
    ImageModel model(&mock);
    model.loadImage("");

    QSignalSpy updatedSignalSpy(&model, SIGNAL(imageUpdated()));

    // TEST
    QVERIFY(model.isImageLoaded());
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());

    // TEST - INVALID
    model.editShadows(-1);
    model.editShadows(51);
    model.editShadows(-9999);
    model.editShadows(9999);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - DEFAULT
    model.editShadows(0);
    QVERIFY(model.getEditedImageQPixmap().toImage() == model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 0);

    // TEST - VALID
    model.editShadows(1);
    model.editShadows(50);
    QVERIFY(model.getEditedImageQPixmap().toImage() != model.getOriginalImageQPixmap().toImage());
    QVERIFY(updatedSignalSpy.length() == 2);
}

QTEST_MAIN(ImageModelTests)
