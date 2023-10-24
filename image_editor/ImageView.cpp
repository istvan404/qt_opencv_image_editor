#include "ImageView.h"


ImageView::ImageView(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Image Editor - OpenCV 4.8.1");
    /*_windowWidth = new int(1280);
    _windowHeight = new int(720);
    resize(*_windowWidth,*_windowHeight);
    setMinimumSize(1280,720);
    setMaximumSize(1920,1080);*/
    setFixedSize(1280, 720);
    qDebug() << "OpenCV current version: " << CV_VERSION;

    _model = new ImageModel(new ImagePersistence(), this);


    _centralWidget = new QWidget();
    setCentralWidget(_centralWidget);

    _layoutMain = new QHBoxLayout();
    _layoutMain->setContentsMargins(0,0,0,0);
    _layoutMain->setSpacing(10);
    _centralWidget->setLayout(_layoutMain);

    // Left side
    _layoutImageContainer = new QVBoxLayout();
    _layoutMain->addLayout(_layoutImageContainer, 66);
    //_layoutImageContainer->addWidget(_imageLabel);

    _imageGraphicsScene = new QGraphicsScene(this);
    _imageGraphicsView = new QGraphicsView(this);
    _layoutImageContainer->addWidget(_imageGraphicsView);


    // Right side
    _editLayout = new QVBoxLayout();
    _layoutMain->addLayout(_editLayout,33);

    // Right side top settings
    _settingsLayout = new QVBoxLayout();
    _editLayout->addLayout(_settingsLayout, 70);
    _settingsLayout->setAlignment(Qt::AlignTop);
    _buttonFlipHorizontal = new QPushButton("Flip Horizontal");
    _buttonFlipVertical = new QPushButton("Flip Vertical");
    _buttonRotate90Plus = new QPushButton("Rotate +90 Degrees");
    _buttonRotate90Minus = new QPushButton("Rotate -90 Degrees");
    _buttonAutoWhiteBalance = new QPushButton("Auto White Balance");
    _buttonZoomIn = new QPushButton("Zoom In");
    _buttonZoomOut = new QPushButton("Zoom Out");
    _buttonZoomFit = new QPushButton("Scale to fit");
    _settingsLayout->addWidget(_buttonFlipHorizontal);
    _settingsLayout->addWidget(_buttonFlipVertical);
    _settingsLayout->addWidget(_buttonRotate90Plus);
    _settingsLayout->addWidget(_buttonRotate90Minus);
    _settingsLayout->addWidget(_buttonAutoWhiteBalance);
    _settingsLayout->addWidget(_buttonZoomIn);
    _settingsLayout->addWidget(_buttonZoomOut);
    _settingsLayout->addWidget(_buttonZoomFit);

    // Right side bottom data
    _detailsLayout = new QVBoxLayout();
    _editLayout->addLayout(_detailsLayout, 30);

    _histogramGraphicsScene = new QGraphicsScene(this);
    _histogramGraphicsView = new QGraphicsView(this);
    _detailsLayout->addWidget(_histogramGraphicsView);


    _menuBar = new QMenuBar(this);
    _fileMenu = new QMenu("File", this);
    _menuBar->addMenu(_fileMenu);

    _actionLoad = new QAction("Load");
    _actionLoad->setStatusTip("Load an image.");
    _fileMenu->addAction(_actionLoad);

    _actionSave = new QAction("Save");
    _actionSave->setShortcut(QKeySequence::fromString("Ctrl+S"));
    _actionSave->setStatusTip("Save the current edited image.");
    _fileMenu->addAction(_actionSave);

    _actionExit = new QAction("Exit");
    _actionExit->setStatusTip("Your progress since the last save will be lost.");
    _fileMenu->addAction(_actionExit);

    // Connect MenuBar Actions To Slots
    connect( _actionLoad, &QAction::triggered, this, &ImageView::onLoadAction);
    connect( _actionSave, &QAction::triggered, this, &ImageView::onSaveAction);
    connect( _actionExit, &QAction::triggered, this, &ImageView::onExitAction);

    // Connect Model Signals To View's Slot
    connect(_model, &ImageModel::imageLoaded, this, &ImageView::onImageModelLoaded);
    connect(_model, &ImageModel::imageUpdated, this, &ImageView::onImageModelUpdated);

    // Connect Right-Top Settings' Buttons To Slots
    connect(_buttonFlipHorizontal, &QPushButton::clicked, this, [this](){
        _model->editFlipHorizontal();
    });
    connect(_buttonFlipVertical, &QPushButton::clicked, this, [this](){
        _model->editFlipVertical();
    });
    connect(_buttonRotate90Plus, &QPushButton::clicked, this, [this](){
        _model->editRotate90Plus();
    });
    connect(_buttonRotate90Minus, &QPushButton::clicked, this, [this](){
        _model->editRotate90Minus();
    });
    connect(_buttonAutoWhiteBalance, &QPushButton::clicked, this, [this](){
        this->setCursor(Qt::CursorShape::BusyCursor);
        _model->editAutoWhiteBalance();
        this->setCursor(Qt::CursorShape::ArrowCursor);
    });
    connect(_buttonZoomIn, SIGNAL(clicked(bool)), this, SLOT(onButtonZoomInClicked()));
    connect(_buttonZoomOut, SIGNAL(clicked(bool)), this, SLOT(onButtonZoomOutClicked()));
    connect(_buttonZoomFit, SIGNAL(clicked(bool)), this, SLOT(onButtonZoomFitClicked()));

    setMenuBar(_menuBar);


    _imageGraphicsScene = new QGraphicsScene(this);
    _histogramGraphicsScene = new QGraphicsScene(this);
}

ImageView::~ImageView()
{
}

void ImageView::reloadImage()
{
    _imageGraphicsScene = new QGraphicsScene(this);
    _imageGraphicsScene->clear();
    _imageGraphicsScene->addPixmap(_model->getEditedImageQPixmap());
    _imageGraphicsView->setScene(_imageGraphicsScene);
    _imageGraphicsView->update();

    _histogramGraphicsScene = new QGraphicsScene(this);
    _histogramGraphicsScene->clear();
    _histogramGraphicsScene->addPixmap(_model->getHistogram(_histogramGraphicsView->size()));
    _histogramGraphicsView->setScene(_histogramGraphicsScene);
    _histogramGraphicsView->update();
}

void ImageView::onButtonZoomInClicked()
{
    if(!_model->isImageLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }

    _imageGraphicsView->scale(1.1,1.1);
}

void ImageView::onButtonZoomOutClicked()
{
    if(!_model->isImageLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }

    _imageGraphicsView->scale(0.9,0.9);

}

void ImageView::onButtonZoomFitClicked()
{
    if(!_model->isImageLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }

    _imageGraphicsView->fitInView(_imageGraphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void ImageView::onImageModelLoaded()
{
    qDebug() << "View catched model's ImageLoaded signal" << QTime::currentTime();
    reloadImage();
    _imageGraphicsView->fitInView(_imageGraphicsScene->sceneRect(), Qt::KeepAspectRatio);
    // Fixing the Histogram bug:
    _histogramGraphicsView->fitInView(_histogramGraphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void ImageView::onImageModelUpdated()
{
    qDebug() << "View catched model's ImageUpdated signal" << QTime::currentTime();
    reloadImage();
}

void ImageView::onLoadAction()
{
    QString path = QFileDialog::getOpenFileName(this, "Select an Image", "", "Images (*.jpg *.png *.bmp)");
    if(path != "") {
        _model->loadImage(path);
    }
}

void ImageView::onSaveAction()
{
    if(!_model->isImageLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }

    QString path = QFileDialog::getSaveFileName(this,
                                                "Save image",
                                                "",
                                                "Images (*.jpg *.png *.bmp)");

    if(path != "")
        _model->saveImage(path);
}

void ImageView::onExitAction()
{
    if(_model->isImageLoaded())
    {
        int answer = QMessageBox::warning(this,
                                          "Image Editor - Warning",
                                          "All progress since the last save will be lost!",
                                          QMessageBox::Ok,
                                          QMessageBox::Cancel);

        if(answer == QMessageBox::Cancel)
            return;
    }

    close();
}
