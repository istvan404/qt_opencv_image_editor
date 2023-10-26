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

    setupMenuBar();

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
    _adjustmentsLayout = new QVBoxLayout();
    _editLayout->addLayout(_adjustmentsLayout, 70);
    _adjustmentsLayout->setAlignment(Qt::AlignTop);
    _buttonAutoWhiteBalance = new QPushButton("Auto White Balance");
    _adjustmentsLayout->addWidget(_buttonAutoWhiteBalance);


    QGridLayout* whiteBalance_grid = new QGridLayout();
    QLabel* whiteBalance_label = new QLabel("White Balance");
    QSlider* whiteBalance_slider = new QSlider(Qt::Orientation::Horizontal);
    QLabel* whiteBalance_min = new QLabel("0");
    QLabel* whiteBalance_max = new QLabel("100");
    QLabel* whiteBalance_count = new QLabel("21");
    QPushButton* whiteBalance_button = new QPushButton("Apply");

    whiteBalance_slider->setMinimum(0);
    whiteBalance_slider->setMaximum(20);
    whiteBalance_slider->setTickInterval(1);
    whiteBalance_slider->setValue(0);
    whiteBalance_slider->setTickPosition(QSlider::TicksAbove);

    QFont font = QFont();
    //font.setBold(true);
    font.setPointSize(12);
    whiteBalance_label->setFont(font);

    whiteBalance_grid->setSpacing(0);
    whiteBalance_grid->addWidget(whiteBalance_label, 0, 0, 1, 4);
    whiteBalance_grid->addWidget(whiteBalance_min, 1, 0, 1, 1);
    whiteBalance_grid->addWidget(whiteBalance_max, 1, 2, 1, 1, Qt::AlignRight);
    whiteBalance_grid->addWidget(whiteBalance_slider, 2, 0, 1, 3);
    whiteBalance_grid->addWidget(whiteBalance_count, 2, 3, 1, 1, Qt::AlignCenter);
    whiteBalance_grid->addWidget(whiteBalance_button, 3, 0, 1, 4);
    /*
    whiteBalance_grid->addWidget(new QPushButton("Label"), 0, 0, 1, 3);
    whiteBalance_grid->addWidget(new QPushButton("Min"), 1, 0, 1, 1);
    whiteBalance_grid->addWidget(new QPushButton("Max"), 1, 2, 1, 1);
    whiteBalance_grid->addWidget(new QPushButton("Slider"), 2, 0, 1, 2);
    whiteBalance_grid->addWidget(new QPushButton("Count"), 2, 2, 1, 1);
    whiteBalance_grid->addWidget(whiteBalance_button, 3, 0, 1, 3);*/

    _adjustmentsLayout->addLayout(whiteBalance_grid);

    // Right side bottom data
    _detailsLayout = new QVBoxLayout();
    _editLayout->addLayout(_detailsLayout, 30);

    _histogramGraphicsScene = new QGraphicsScene(this);
    _histogramGraphicsView = new QGraphicsView(this);
    _detailsLayout->addWidget(_histogramGraphicsView);



    // Connect Model Signals To View's Slot
    connect(_model, &ImageModel::imageLoaded, this, &ImageView::onImageModelLoaded);
    connect(_model, &ImageModel::imageUpdated, this, &ImageView::onImageModelUpdated);

    // Connect Right-Top Settings' Buttons To Slots
    connect(_buttonAutoWhiteBalance, &QPushButton::clicked, this, [this](){
        this->setCursor(Qt::CursorShape::BusyCursor);
        _model->editAutoWhiteBalance(10);
        this->setCursor(Qt::CursorShape::ArrowCursor);
    });

    setMenuBar(_menuBar);

    _imageGraphicsScene = new QGraphicsScene(this);
    _histogramGraphicsScene = new QGraphicsScene(this);
}

ImageView::~ImageView()
{
}

void ImageView::setupMenuBar()
{
    // Initialization
    _menuBar = new QMenuBar(this);

    setupMenuFile();
    setupMenuView();
}

void ImageView::setupMenuFile()
{
    // Initialization
    _menuFile = new QMenu("File", this);
    _actionLoad = new QAction("Load");
    _actionLoad->setStatusTip("Load an image.");
    _actionSave = new QAction("Save");
    _actionSave->setShortcut(QKeySequence::fromString("Ctrl+S"));
    _actionSave->setStatusTip("Save the current edited image.");
    _actionExit = new QAction("Exit");
    _actionExit->setStatusTip("Your progress since the last save will be lost.");

    // Adding to viewport
    _menuBar->addMenu(_menuFile);
    _menuFile->addAction(_actionLoad);
    _menuFile->addAction(_actionSave);
    _menuFile->addAction(_actionExit);

    // Connect SIGNALs to SLOTs
    connect( _actionLoad, SIGNAL(triggered(bool)), this, SLOT(onActionLoad()));
    connect( _actionSave, SIGNAL(triggered(bool)), this, SLOT(onActionSave()));
    connect( _actionExit, SIGNAL(triggered(bool)), this, SLOT(onActionExit()));
}

void ImageView::setupMenuView()
{
    // Initialization
    _menuView = new QMenu("View", this);
    _actionZoomIn = new QAction("Zoom In");
    _actionZoomIn->setShortcut(QKeySequence::fromString("Ctrl+I"));
    _actionZoomOut = new QAction("Zoom Out");
    _actionZoomOut->setShortcut(QKeySequence::fromString("Ctrl+O"));
    _actionZoomFit = new QAction("Scale to fit");
    _actionFlipHorizontal = new QAction("Flip Horizontal");
    _actionFlipVertical = new QAction("Flip Vertical");
    _actionRotate90CW = new QAction("Rotate 90 CW");
    _actionRotate90CW->setShortcut(QKeySequence::fromString("Ctrl+R"));
    _actionRotate90CCW = new QAction("Rotate 90 CCW");
    _actionRotate90CCW->setShortcut(QKeySequence::fromString("Ctrl+Shift+R"));
    _actionRotate180 = new QAction("Rotate 180 degrees");

    // Adding to viewport
    _menuBar->addMenu(_menuView);
    _menuView->addAction(_actionZoomIn);
    _menuView->addAction(_actionZoomOut);
    _menuView->addAction(_actionZoomFit);
    _menuView->addSeparator();
    _menuView->addAction(_actionFlipHorizontal);
    _menuView->addAction(_actionFlipVertical);
    _menuView->addSeparator();
    _menuView->addAction(_actionRotate90CW);
    _menuView->addAction(_actionRotate90CCW);
    _menuView->addAction(_actionRotate180);

    // Connect SIGNALs to SLOTs
    connect(_actionZoomIn,          SIGNAL(triggered(bool)), this, SLOT(onActionZoomIn()));
    connect(_actionZoomOut,         SIGNAL(triggered(bool)), this, SLOT(onActionZoomOut()));
    connect(_actionZoomFit,         SIGNAL(triggered(bool)), this, SLOT(onActionZoomFit()));
    connect(_actionFlipHorizontal,  SIGNAL(triggered(bool)), this, SLOT(onActionFlipHorizontal()));
    connect(_actionFlipVertical,    SIGNAL(triggered(bool)), this, SLOT(onActionFlipVertical()));
    connect(_actionRotate90CW,      SIGNAL(triggered(bool)), this, SLOT(onActionRotate90CW()));
    connect(_actionRotate90CCW,     SIGNAL(triggered(bool)), this, SLOT(onActionRotate90CCW()));
    connect(_actionRotate180,       SIGNAL(triggered(bool)), this, SLOT(onActionRotate180()));
}

void ImageView::loadImage()
{
    if(_imageGraphicsScene != nullptr)
        delete _imageGraphicsScene;

    _imageGraphicsScene = new QGraphicsScene(this);
    _imageGraphicsScene->clear();
    _imageGraphicsScene->addPixmap(_model->getEditedImageQPixmap());
    _imageGraphicsView->setScene(_imageGraphicsScene);
    _imageGraphicsView->update();

    if(_histogramGraphicsScene != nullptr)
        delete _histogramGraphicsScene;

    _histogramGraphicsScene = new QGraphicsScene(this);
    _histogramGraphicsScene->clear();
    _histogramGraphicsScene->addPixmap(_model->getHistogram(_histogramGraphicsView->size()));
    _histogramGraphicsView->setScene(_histogramGraphicsScene);
    _histogramGraphicsView->update();
}

void ImageView::onActionZoomIn()
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

void ImageView::onActionZoomOut()
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

void ImageView::onActionZoomFit()
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

void ImageView::onActionFlipHorizontal()
{
    if(!_model->isImageLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }
    _model->editFlipHorizontal();
}

void ImageView::onActionFlipVertical()
{
    if(!_model->isImageLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }
    _model->editFlipVertical();
}

void ImageView::onActionRotate90CW()
{
    if(!_model->isImageLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }
    _model->editRotate(90);
    _imageGraphicsView->fitInView(_imageGraphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void ImageView::onActionRotate90CCW()
{
    if(!_model->isImageLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }
    _model->editRotate(-90);
    _imageGraphicsView->fitInView(_imageGraphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void ImageView::onActionRotate180()
{
    if(!_model->isImageLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }
    _model->editRotate(180);
    _imageGraphicsView->fitInView(_imageGraphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void ImageView::onImageModelLoaded()
{
    qDebug() << "View catched model's ImageLoaded signal" << QTime::currentTime();
    loadImage();
    _imageGraphicsView->fitInView(_imageGraphicsScene->sceneRect(), Qt::KeepAspectRatio);
    // Fixing the Histogram bug:
    _histogramGraphicsView->fitInView(_histogramGraphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void ImageView::onImageModelUpdated()
{
    qDebug() << "View catched model's ImageUpdated signal" << QTime::currentTime();
    loadImage();
}

void ImageView::onActionLoad()
{
    QString path = QFileDialog::getOpenFileName(this, "Select an Image", "", "Images (*.jpg *.png *.bmp)");
    if(path != "") {
        _model->loadImage(path);
    }
}

void ImageView::onActionSave()
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

void ImageView::onActionExit()
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
