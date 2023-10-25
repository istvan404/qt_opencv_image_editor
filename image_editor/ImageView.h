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

    // Right-Top Settings
    QVBoxLayout* _settingsLayout;
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
    QMenu* _fileMenu;
    QAction* _actionLoad;
    QAction* _actionSave;
    QAction* _actionExit;

    void reloadImage();

private slots:
    // MenuBar Actions' slots
    void onLoadAction();
    void onSaveAction();
    void onExitAction();
    // Model's signal slot
    void onImageModelLoaded();
    void onImageModelUpdated();
    // Settings button slots
    void onButtonZoomInClicked();
    void onButtonZoomOutClicked();
    void onButtonZoomFitClicked();
    //void onButtonFlipHorizontal(); // ?
    //void onButtonFlipVertical(); // ?
};
#endif // IMAGEVIEW_H
