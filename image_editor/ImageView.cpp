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
    _imageLabel = new QLabel();
    _imageLabel->setAlignment(Qt::AlignCenter);
    _layoutImageContainer = new QVBoxLayout();
    _layoutMain->addLayout(_layoutImageContainer, 66);
    _layoutImageContainer->addWidget(_imageLabel);
    //qDebug() << _imageLabel->size().width();

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
    _settingsLayout->addWidget(_buttonFlipHorizontal);
    _settingsLayout->addWidget(_buttonFlipVertical);
    _settingsLayout->addWidget(_buttonRotate90Plus);
    _settingsLayout->addWidget(_buttonRotate90Minus);
    _settingsLayout->addWidget(_checkboxToggleScale);
    _checkboxToggleScale->setCheckState(Qt::Checked);

    // Right side bottom data
    _detailsLayout = new QVBoxLayout();
    _editLayout->addLayout(_detailsLayout, 30);
    _detailsLayout->setSpacing(0);
    _labelHistogram = new QLabel();
    _labelHistogram->setAlignment(Qt::AlignCenter);
    _detailsLayout->addWidget(_labelHistogram);


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

    // Connect Model Signals To View
    connect(_model, &ImageModel::imageLoaded, this, &ImageView::onImageLoaded);

    // Connect Right-Top Settings' Buttons To Slots
    connect(_buttonFlipHorizontal, &QPushButton::clicked, this, [this](){ _model->editFlipHorizontal(); });
    connect(_buttonFlipVertical, &QPushButton::clicked, this, [this](){ _model->editFlipVertical(); });
    connect(_buttonRotate90Plus, &QPushButton::clicked, this, [this](){ _model->editRotate90Plus(); });
    connect(_buttonRotate90Minus, &QPushButton::clicked, this, [this](){ _model->editRotate90Minus(); });
    connect(_checkboxToggleScale, &QCheckBox::stateChanged, this, [this](){ _model->editToggleImageScale( _checkboxToggleScale->isChecked() ); });

    setMenuBar(_menuBar);

    qDebug() << "Main layout size: " << _layoutMain->sizeHint();
    qDebug() << "ImageContainer layout size: " << _layoutImageContainer->sizeHint();
    qDebug() << "_editLayout size: " << _editLayout->sizeHint();
    qDebug() << "_imageLabel size: " << _imageLabel->size();
    //_imageLabel->setFixedSize(_imageLabel->size());
}

ImageView::~ImageView()
{
}

void ImageView::onImageLoaded()
{
    _imageLabel->setFixedSize(_imageLabel->size());
    _imageLabel->setPixmap( _model->getEditedImageQPixmap( _imageLabel->size() ) );

    _labelHistogram->setFixedSize(_labelHistogram->size());
    _labelHistogram->setPixmap(_model->getHistogram(_labelHistogram->size()));
}

void ImageView::onLoadAction()
{
    QString path = QFileDialog::getOpenFileName(this, "Open image", "", "Images (*.JPG *.jpg *.png *.bmp)");
    if(path != "") {
        _model->loadImage(path);
    }
}

void ImageView::onSaveAction()
{
    if(!_model->isImageLoaded())
    {
        qDebug() << "Model's image is not loaded yet.";
        return;
    }

    QString path = QFileDialog::getSaveFileName(this, "Save image", "", "Images (*.JPG *.jpg *.png *.bmp)");
    if(path != "") {
        _model->saveImage(path);
    }
}

void ImageView::onExitAction()
{
    close();
}
