#include "ImageView.h"

#include <QMenuBar>

ImageView::ImageView(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "OpenCV current version: " << CV_VERSION;

    _menuBar = new QMenuBar(this);
    _fileMenu = new QMenu("File", this);
    _menuBar->addMenu(_fileMenu);


    _actionLoad = new QAction("Load");
    _actionLoad->setStatusTip("Load an image.");
    _fileMenu->addAction(_actionLoad);

    _actionSave = new QAction("Save");
    //_actionSave->setShortcut(QKeySequence::fromString("Ctrl+S"));
    _actionSave->setStatusTip("Save the current edited image.");
    _fileMenu->addAction(_actionSave);

    _actionExit = new QAction("Exit");
    _actionExit->setStatusTip("Your progress since the last save will be lost.");
    _fileMenu->addAction(_actionExit);


    connect( _actionLoad, &QAction::triggered, this, &ImageView::onLoadAction);
    connect( _actionSave, &QAction::triggered, this, &ImageView::onSaveAction);
    connect( _actionExit, &QAction::triggered, this, &ImageView::onExitAction);

    setMenuBar(_menuBar);
}

ImageView::~ImageView()
{
}

void ImageView::onLoadAction()
{
    QString path = QFileDialog::getOpenFileName(this, "Open image.", "", "Images (*.JPG)");
    if(path != "") {
        qDebug() << "Loading file from " << path;

        cv::Mat image = cv::imread(path.toStdString());
        cv::namedWindow("Image");
        cv::imshow("Image", image);
    }
}

void ImageView::onSaveAction()
{
    QString path = QFileDialog::getSaveFileName(this, "Save image.", "", "Images (*.jpg, *.JPG, *.png, *.bmp)");
    if(path != "") {
        qDebug() << "Saving file to " << path;
    }
}

void ImageView::onExitAction()
{
    close();
}
