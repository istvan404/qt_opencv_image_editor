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

    _mainLayout = new QHBoxLayout();
    _mainLayout->setContentsMargins(0,0,0,0);
    _mainLayout->setSpacing(10);
    _centralWidget->setLayout(_mainLayout);

    // Left side
    _imageLabel = new QLabel();
    _imageLabel->setAlignment(Qt::AlignCenter);
    _mainLayout->addWidget(_imageLabel,66);
    //qDebug() << _imageLabel->size().width();

    // Right side
    _editLayout = new QVBoxLayout();
    _mainLayout->addLayout(_editLayout,33);

    // Right side top settings
    _settingsLayout = new QVBoxLayout();
    _editLayout->addLayout(_settingsLayout);
    _settingsLayout->setAlignment(Qt::AlignTop);
    _buttonFlipHorizontal = new QPushButton("Flip Horizontal");
    _buttonFlipVertical = new QPushButton("Flip Vertical");
    _buttonRotate90Plus = new QPushButton("Rotate +90 Degrees");
    _buttonRotate90Minus = new QPushButton("Rotate -90 Degrees");
    _settingsLayout->addWidget(_buttonFlipHorizontal);
    _settingsLayout->addWidget(_buttonFlipVertical);
    _settingsLayout->addWidget(_buttonRotate90Plus);
    _settingsLayout->addWidget(_buttonRotate90Minus);

    // Right side bottom data
    _detailsLayout = new QVBoxLayout();
    _editLayout->addLayout(_detailsLayout);


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


    setMenuBar(_menuBar);
}

ImageView::~ImageView()
{
}



void ImageView::onImageLoaded()
{
    qDebug() << "ImageView received the ImageModel's imageLoaded signal!";

    _imageLabel->setPixmap( _model->getEditedImageQPixmap( _imageLabel->size() ) );

    //resize(*_windowWidth,*_windowHeight);
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
