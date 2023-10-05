#include "imagewindow.h"

ImageWindow::ImageWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "OpenCV version: " << CV_VERSION;
}

ImageWindow::~ImageWindow()
{
}

