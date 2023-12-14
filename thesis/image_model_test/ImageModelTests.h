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
    const QString test_file_path = "../../thesis/image_model_test/images/test.jpg";

private slots:
    void Test_Initialization_Empty();
    void Test_Initialization_TestFile();
    void Test_Signals();
    void Test_Flip_Vertical();
    void Test_Flip_Horizontal();
    void Test_Flip_Mixed();
    void Test_Rotate_90_CW();
    void Test_Rotate_90_CCW();
    void Test_Rotate_180();
    void Test_Rotate_Mixed();
    void Test_Rotate_Invalid_Values();
    void Test_Nothing_Happens_Without_Image();
    void Test_Reset();
    void Test_Brightness_Values();
    //void Test_WhiteBalance_Values();
    //void Test_ShadowBasic_Values();
    //void Test_Shadows_Values();
};

#endif // IMAGEMODELTESTS_H
