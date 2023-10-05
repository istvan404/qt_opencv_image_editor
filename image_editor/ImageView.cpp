#include "ImageView.h"

ImageView::ImageView(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "OpenCV current version: " << CV_VERSION;
}

ImageView::~ImageView()
{
}

