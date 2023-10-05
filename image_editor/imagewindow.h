#ifndef IMAGEWINDOW_H
#define IMAGEWINDOW_H

#include <QMainWindow>

class ImageWindow : public QMainWindow
{
    Q_OBJECT

public:
    ImageWindow(QWidget *parent = nullptr);
    ~ImageWindow();
};
#endif // IMAGEWINDOW_H
