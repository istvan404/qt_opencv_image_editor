#ifndef IMAGEMODELTESTS_H
#define IMAGEMODELTESTS_H

#include <QObject>
#include <QtTest>
#include <QCoreApplication>

class ImageModelTests : public QObject
{
    Q_OBJECT
public:
    explicit ImageModelTests(QObject *parent = nullptr);

private:
    const QString path_jpg = "../../thesis/image_model_test/images/test_one.jpg";
    const QString path_png = "../../thesis/image_model_test/images/test_two.png";
    const QString path_bmp = "../../thesis/image_model_test/images/test_three.bmp";
    const QString path_invalid = "../../thesis/image_model_test/images/invalid_test.txt";

private slots:
    void Test_Initialization_Empty();
    void Test_File_JPG();
    void Test_File_PNG();
    void Test_File_BMP();
    void Test_Invalid_File_Path();
    void Test_Signals();
    void Test_Nothing_Happens_Without_Image();
    void Test_Reset();
    void Test_Flip_Vertical();
    void Test_Flip_Horizontal();
    void Test_Flip_Mixed();
    void Test_Rotate_90_CW();
    void Test_Rotate_90_CCW();
    void Test_Rotate_180();
    void Test_Rotate_Mixed();
    void Test_Rotate_Invalid_Values();
    void Test_Brightness_Values();
    void Test_WhiteBalance_Values();
    void Test_WhiteBalance_GW();
    void Test_ShadowBasic_Values();
    void Test_Shadows_Values();
};

#endif // IMAGEMODELTESTS_H
