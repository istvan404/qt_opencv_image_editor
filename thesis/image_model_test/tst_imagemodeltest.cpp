#include <QtTest>

// add necessary includes here

class ImageModelTest : public QObject
{
    Q_OBJECT

public:
    ImageModelTest();
    ~ImageModelTest();

private slots:
    void test_case1();

};

ImageModelTest::ImageModelTest()
{

}

ImageModelTest::~ImageModelTest()
{

}

void ImageModelTest::test_case1()
{

}

//QTEST_APPLESS_MAIN(ImageModelTest)

#include "tst_imagemodeltest.moc"
