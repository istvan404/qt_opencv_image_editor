#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QMainWindow>

#include <QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class ImageWindow : public QMainWindow
{
    Q_OBJECT

public:
    ImageWindow(QWidget *parent = nullptr);
    ~ImageWindow();
};
#endif // IMAGEWINDOW_H
