#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "ImageModel.h"
#include <QMainWindow>
#include <QMenuBar>
#include <QFileDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>

#include <QDebug>

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
    QLabel* _imageLabel;

    // { #Right side
    QVBoxLayout* _editLayout;

    // Right-Top Settings
    QVBoxLayout* _settingsLayout;
    QPushButton* _buttonFlipHorizontal;
    QPushButton* _buttonFlipVertical;
    QPushButton* _buttonRotate90Plus;
    QPushButton* _buttonRotate90Minus;
    QCheckBox* _checkboxToggleScale;


    // Right-Bottom Details
    QVBoxLayout* _detailsLayout;
    QLabel* _labelHistogram;
    // }

    // Menu
    QMenuBar* _menuBar;

    // File
    QMenu* _fileMenu;
    QAction* _actionLoad;
    QAction* _actionSave;
    QAction* _actionExit;

private slots:
    // MenuBar Actions' slots
    void onLoadAction();
    void onSaveAction();
    void onExitAction();
    // Model's signal slot
    void onImageLoaded();
    // Settings button slots
    //void onButtonFlipHorizontal(); // ?
    //void onButtonFlipVertical(); // ?
};
#endif // IMAGEVIEW_H
