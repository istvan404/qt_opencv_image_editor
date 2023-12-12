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

private slots:
    void Test_Initialization();
    //void Test_Load();
    //void Test_Flip();
    //void Test_Rotate();
};

#endif // IMAGEMODELTESTS_H
