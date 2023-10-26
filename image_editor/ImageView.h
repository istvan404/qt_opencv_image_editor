#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "ImageModel.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QMenuBar>
#include <QFileDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCursor>
#include <QSlider>

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class ImageView : public QMainWindow
{
    Q_OBJECT

public:
    ImageView(QWidget *parent = nullptr);
    ~ImageView();

private:

    ImageModel* _model;

    /*int* _windowWidth;
    int* _windowHeight;*/

    QWidget* _centralWidget;
    QHBoxLayout* _layoutMain;

    // Left side
    QVBoxLayout* _layoutImageContainer;
    QGraphicsScene* _imageGraphicsScene;
    QGraphicsView* _imageGraphicsView;

    // { #Right side
    QVBoxLayout* _editLayout;

    // Right-Top Adjustments
    QVBoxLayout* _adjustmentsLayout;
    QPushButton* _buttonFlipHorizontal;
    QPushButton* _buttonFlipVertical;
    QPushButton* _buttonRotate90Plus;
    QPushButton* _buttonRotate90Minus;
    QPushButton* _buttonAutoWhiteBalance;
    QPushButton* _buttonZoomOut;
    QPushButton* _buttonZoomIn;
    QPushButton* _buttonZoomFit;


    // Right-Bottom Details
    QVBoxLayout* _detailsLayout;
    QGraphicsScene* _histogramGraphicsScene;
    QGraphicsView* _histogramGraphicsView;
    // }

    // Menu
    QMenuBar* _menuBar;

    // File
    QMenu* _menuFile;
    QAction* _actionLoad;
    QAction* _actionSave;
    QAction* _actionExit;

    // File
    QMenu* _menuView;
    QAction* _actionZoomIn;
    QAction* _actionZoomOut;
    QAction* _actionZoomFit;
    QAction* _actionFlipHorizontal;
    QAction* _actionFlipVertical;
    QAction* _actionRotate90CW;
    QAction* _actionRotate90CCW;
    QAction* _actionRotate180;

    void setupMenuBar();
    void setupMenuFile();
    void setupMenuView();
    void loadImage();

private slots:
    // MenuBar Actions' slots
    void onActionLoad();
    void onActionSave();
    void onActionExit();
    void onActionZoomIn();
    void onActionZoomOut();
    void onActionZoomFit();
    void onActionFlipHorizontal();
    void onActionFlipVertical();
    void onActionRotate90CW();
    void onActionRotate90CCW();
    void onActionRotate180();

    // Model's signal slot
    void onImageModelLoaded();
    void onImageModelUpdated();

    // Adjustments button slots
    void onButtonZoomInClicked();
    void onButtonZoomOutClicked();
    void onButtonZoomFitClicked();
    //void onButtonFlipHorizontal(); // ?
    //void onButtonFlipVertical(); // ?
};
#endif // IMAGEVIEW_H
