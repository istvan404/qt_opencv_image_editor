#include "ImageView.h"


ImageView::ImageView(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Image Editor w/ OpenCV 4.8.1");
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
    _checkboxToggleScale = new QCheckBox("Image scale to fit space");
    _buttonAutoWhiteBalance = new QPushButton("Auto White Balance");
    _buttonZoomIn = new QPushButton("Zoom In");
    _buttonZoomOut = new QPushButton("Zoom Out");
    _settingsLayout->addWidget(_buttonFlipHorizontal);
    _settingsLayout->addWidget(_buttonFlipVertical);
    _settingsLayout->addWidget(_buttonRotate90Plus);
    _settingsLayout->addWidget(_buttonRotate90Minus);
    _settingsLayout->addWidget(_checkboxToggleScale);
    _checkboxToggleScale->setCheckable(false);
    _settingsLayout->addWidget(_buttonAutoWhiteBalance);
    _settingsLayout->addWidget(_buttonZoomIn);
    _settingsLayout->addWidget(_buttonZoomOut);

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
    connect(_model, &ImageModel::imageLoaded, this, &ImageView::onImageLoaded);

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
    connect(_checkboxToggleScale, &QCheckBox::clicked, this, [this](){
        _model->editToggleImageScale( _checkboxToggleScale->isChecked() );
    });
    connect(_buttonAutoWhiteBalance, &QPushButton::clicked, this, [this](){
        this->setCursor(Qt::CursorShape::BusyCursor);
        _model->editAutoWhiteBalance();
        this->setCursor(Qt::CursorShape::ArrowCursor);
    });
    connect(_buttonZoomIn, SIGNAL(clicked(bool)), this, SLOT(onButtonZoomInClicked()));
    connect(_buttonZoomOut, SIGNAL(clicked(bool)), this, SLOT(onButtonZoomOutClicked()));

    setMenuBar(_menuBar);
}

ImageView::~ImageView()
{
}

void ImageView::onButtonZoomInClicked()
{
    qDebug() << "Zoom In Clicked";
    _imageGraphicsView->scale(1.1,1.1);
}

void ImageView::onButtonZoomOutClicked()
{
    qDebug() << "Zoom Out Clicked";
    _imageGraphicsView->scale(0.9,0.9);
}

void ImageView::onImageLoaded()
{
    qDebug() << "onImageLoaded emited at " << QTime::currentTime();

    _imageGraphicsScene = new QGraphicsScene(this);
    _imageGraphicsScene->addPixmap(_model->getEditedImageQPixmap( _imageGraphicsView->size() ));
    _imageGraphicsView->setScene(_imageGraphicsScene);

    _histogramGraphicsScene = new QGraphicsScene(this);
    _histogramGraphicsScene->addPixmap(_model->getHistogram(_histogramGraphicsView->size()));
    _histogramGraphicsView->setScene(_histogramGraphicsScene);

    _checkboxToggleScale->setCheckable(true);
}

void ImageView::onLoadAction()
{
    QString path = QFileDialog::getOpenFileName(this, "Select an Image", "", "Images (*.jpg *.png *.bmp)");
    if(path != "") {
        _model->loadImage(path);
        _checkboxToggleScale->setCheckState(Qt::Checked);
    }
}

void ImageView::onSaveAction()
{
    if(!_model->isImageLoaded())
    {
        qDebug() << "Model's image is not loaded yet.";
        return;
    }

    QString path = QFileDialog::getSaveFileName(this, "Save image", "", "Images (*.jpg *.png *.bmp)");
    if(path != "") {
        _model->saveImage(path);
    }
}

void ImageView::onExitAction()
{
    close();
}
