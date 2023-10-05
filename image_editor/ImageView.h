#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QMainWindow>

#include <QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class ImageView : public QMainWindow
{
    Q_OBJECT

public:
    ImageView(QWidget *parent = nullptr);
    ~ImageView();
};
#endif // IMAGEVIEW_H
