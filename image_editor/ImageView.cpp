#include "ImageView.h"

#include <QMenuBar>

ImageView::ImageView(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Image Editor w/ OpenCV 4.8.1");
    setFixedSize(1280, 720);
    qDebug() << "OpenCV current version: " << CV_VERSION;

    _model = new ImageModel(new ImagePersistence(), this);


    _centralWidget = new QWidget();
    setCentralWidget(_centralWidget);

    _mainLayout = new QVBoxLayout();
    _centralWidget->setLayout(_mainLayout);

    _label = new QLabel();
    _mainLayout->addWidget(_label);
    //_label->setPixmap( QPixmap("../demo.jpg") );

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

    connect(_model, &ImageModel::imageLoaded, this, &ImageView::onImageLoaded);


    setMenuBar(_menuBar);
}

ImageView::~ImageView()
{
}

void ImageView::onImageLoaded()
{
    qDebug() << "ImageView received the ImageModel's imageLoaded signal!";

    _label->setPixmap( _model->getQPixmap() );
}

void ImageView::onLoadAction()
{
    QString path = QFileDialog::getOpenFileName(this, "Open image", "", "Images (*.JPG)");
    if(path != "") {
        qDebug() << "Loading file from " << path;

        if(!_model->loadImage(path))
        {
            qDebug() << "Error while loading file in ImageModel!";
        }

        /*
        cv::Mat image = cv::imread(path.toStdString());
        cv::namedWindow("Image");
        cv::imshow("Image", image);
        */
    }
}

void ImageView::onSaveAction()
{
    QString path = QFileDialog::getSaveFileName(this, "Save image", "", "Images (*.JPG)");
    if(path != "") {
        qDebug() << "Saving file to " << path;
    }
}

void ImageView::onExitAction()
{
    close();
}
