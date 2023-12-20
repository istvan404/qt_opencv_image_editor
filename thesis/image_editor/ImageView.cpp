#include "ImageView.h"

ImageView::ImageView(QWidget *parent)
    : QMainWindow(parent)
{
    QString title = "Image Editor";
    title += " - OpenCV:";
    title += CV_VERSION;

    setWindowTitle(title);
    setFixedSize(1280, 720);

    _model = new ImageModel(new ImagePersistence(), this);

    setupCentralWidget();
    setupMenuBar();
    setupImageViewport();
    setupSide();
    connectModel();
}

ImageView::~ImageView()
{
}

void ImageView::connectModel()
{
    // Connect Model Signals To View's Slot
    connect(_model, &ImageModel::imageLoaded, this, &ImageView::onImageModelLoaded);
    connect(_model, &ImageModel::imageUpdated, this, &ImageView::onImageModelUpdated);
}

void ImageView::setupCentralWidget()
{
    _centralWidget = new QWidget();
    setCentralWidget(_centralWidget);

    _layoutMain = new QHBoxLayout();
    _layoutMain->setContentsMargins(0,0,0,0);
    _layoutMain->setSpacing(10);
    _centralWidget->setLayout(_layoutMain);
}

void ImageView::setupImageViewport()
{
    // Left side
    _layoutImageContainer = new QVBoxLayout();
    _layoutMain->addLayout(_layoutImageContainer, 66);

    _imageGraphicsScene = new QGraphicsScene(this);
    _imageGraphicsView = new QGraphicsView(this);
    _layoutImageContainer->addWidget(_imageGraphicsView);
}

void ImageView::setupSide()
{
    // Right side
    _layoutSide = new QVBoxLayout();
    _layoutMain->addLayout(_layoutSide,33);

    setupAdjustments();
    setupHistogram();
}

void ImageView::setupAdjustments()
{
    // Initialization
    _layoutAdjustments =            new QVBoxLayout();
    _labelAdjustmentsTitle =        new QLabel("Adjustments");
    _buttonAdjustmentsReset =       new QPushButton("Reset");
    _adjustmentWhiteBalance =       new Adjustment("White Balance - Basic Algorithm", 0, 20, 0);
    _adjustmentWhiteBalanceGW =     new Adjustment("White Balance - Gray World Algorithm");
    _adjustmentBrightness =         new Adjustment("Brightness", -50, 50, 0);
    _adjustmentShadowBasic =        new Adjustment("Shadow - Basic", 0, 50, 0);
    _adjustmentShadow =             new Adjustment("Shadow - With Mask", 0, 50, 0);

    // Personalization
    _layoutAdjustments->setAlignment(Qt::AlignTop);
    _layoutAdjustments->setContentsMargins(0, 10, 0, 10);
    QFont title = QFont();
    title.setPointSize(10);
    title.setBold(true);
    _labelAdjustmentsTitle->setFont(title);

    // Display
    _layoutSide->addLayout(_layoutAdjustments, 70);
    _layoutAdjustments->addWidget(_labelAdjustmentsTitle);
    _layoutAdjustments->addWidget(_buttonAdjustmentsReset);
    _layoutAdjustments->addLayout(_adjustmentWhiteBalance);
    _layoutAdjustments->addLayout(_adjustmentWhiteBalanceGW);
    _layoutAdjustments->addLayout(_adjustmentBrightness);
    _layoutAdjustments->addLayout(_adjustmentShadowBasic);
    _layoutAdjustments->addLayout(_adjustmentShadow);

    // Connect SIGNALs to SLOTs
    connect(_buttonAdjustmentsReset,                SIGNAL(clicked(bool)), this, SLOT(onAdjustmentsResetButtonClicked()));
    connect(_adjustmentWhiteBalance->button(),      SIGNAL(clicked(bool)), this, SLOT(onAdjustmentWhiteBalanceClicked()));
    connect(_adjustmentWhiteBalanceGW->button(),    SIGNAL(clicked(bool)), this, SLOT(onAdjustmentWhiteBalanceGWClicked()));
    connect(_adjustmentBrightness->button(),        SIGNAL(clicked(bool)), this, SLOT(onAdjustmentBrightnessClicked()));
    connect(_adjustmentShadowBasic->button(),       SIGNAL(clicked(bool)), this, SLOT(onAdjustmentShadowBasicClicked()));
    connect(_adjustmentShadow->button(),            SIGNAL(clicked(bool)), this, SLOT(onAdjustmentShadowClicked()));
}

void ImageView::onAdjustmentsResetButtonClicked()
{
    if(!_model->isImageDataLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }

    _model->editReset();
}

void ImageView::onAdjustmentWhiteBalanceClicked()
{
    if(!_model->isImageDataLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }

    this->setCursor(Qt::CursorShape::BusyCursor);
    _model->editWhiteBalance(_adjustmentWhiteBalance->value());
    this->setCursor(Qt::CursorShape::ArrowCursor);
}

void ImageView::onAdjustmentWhiteBalanceGWClicked()
{
    if(!_model->isImageDataLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }

    this->setCursor(Qt::CursorShape::BusyCursor);
    _model->editWhiteBalanceGW();
    this->setCursor(Qt::CursorShape::ArrowCursor);
}

void ImageView::onAdjustmentBrightnessClicked()
{
    if(!_model->isImageDataLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }

    this->setCursor(Qt::CursorShape::BusyCursor);
    _model->editBrightness(_adjustmentBrightness->value());
    this->setCursor(Qt::CursorShape::ArrowCursor);
}

void ImageView::onAdjustmentShadowBasicClicked()
{
    if(!_model->isImageDataLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }

    this->setCursor(Qt::CursorShape::BusyCursor);
    _model->editShadowsBasic(_adjustmentShadowBasic->value());
    this->setCursor(Qt::CursorShape::ArrowCursor);
}

void ImageView::onAdjustmentShadowClicked()
{
    if(!_model->isImageDataLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }

    this->setCursor(Qt::CursorShape::BusyCursor);
    _model->editShadows(_adjustmentShadow->value());
    this->setCursor(Qt::CursorShape::ArrowCursor);
}

void ImageView::setupHistogram()
{
    _layoutHistogram = new QVBoxLayout();
    _layoutSide->addLayout(_layoutHistogram, 30);

    _histogramGraphicsScene = new QGraphicsScene(this);
    _histogramGraphicsView = new QGraphicsView(this);
    _layoutHistogram->addWidget(_histogramGraphicsView);
}

void ImageView::setupMenuBar()
{
    // Initialization
    _menuBar = new QMenuBar(this);

    setupMenuFile();
    setupMenuView();

    setMenuBar(_menuBar);
}

void ImageView::setupMenuFile()
{
    // Initialization
    _menuFile =     new QMenu("File", this);
    _actionLoad =   new QAction("Load");
    _actionSave =   new QAction("Save");
    _actionExit =   new QAction("Exit");

    // Personalization
    _actionLoad->setStatusTip("Load an image.");
    _actionSave->setShortcut(QKeySequence::fromString("Ctrl+S"));
    _actionSave->setStatusTip("Save the current edited image.");
    _actionExit->setStatusTip("Your progress since the last save will be lost.");

    // Display
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
    _menuView =             new QMenu("View", this);
    _actionZoomIn =         new QAction("Zoom In");
    _actionZoomOut =        new QAction("Zoom Out");
    _actionZoomFit =        new QAction("Scale to fit");
    _actionFlipHorizontal = new QAction("Flip Horizontal");
    _actionFlipVertical =   new QAction("Flip Vertical");
    _actionRotate90CW =     new QAction("Rotate 90 CW");
    _actionRotate90CCW =    new QAction("Rotate 90 CCW");
    _actionRotate180 =      new QAction("Rotate 180 degrees");

    // Personalization
    _actionZoomIn->setShortcut(QKeySequence::fromString("Ctrl+I"));
    _actionZoomOut->setShortcut(QKeySequence::fromString("Ctrl+O"));
    _actionRotate90CW->setShortcut(QKeySequence::fromString("Ctrl+R"));
    _actionRotate90CCW->setShortcut(QKeySequence::fromString("Ctrl+Shift+R"));

    // Display
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
    if(!_model->isImageDataLoaded())
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
    if(!_model->isImageDataLoaded())
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
    if(!_model->isImageDataLoaded())
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
    if(!_model->isImageDataLoaded())
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
    if(!_model->isImageDataLoaded())
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
    if(!_model->isImageDataLoaded())
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
    if(!_model->isImageDataLoaded())
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
    if(!_model->isImageDataLoaded())
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
    loadImage();
    _imageGraphicsView->fitInView(_imageGraphicsScene->sceneRect(), Qt::KeepAspectRatio);
    _histogramGraphicsView->fitInView(_histogramGraphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void ImageView::onImageModelUpdated()
{
    loadImage();
}

void ImageView::onActionLoad()
{
    if(_model->isImageDataLoaded())
    {
        int answer = QMessageBox::warning(this,
                                          "Image Editor - Warning",
                                          "There is an image already loaded. If you continue, the current image won't be saved!",
                                          QMessageBox::Ok,
                                          QMessageBox::Cancel);

        if(answer == QMessageBox::Cancel)
            return;
    }

    QString path = QFileDialog::getOpenFileName(this, "Select an Image", "", "Images (*.jpg *.png *.bmp)");
    if(path != "") {
        _model->loadImage(path);
    }
}

void ImageView::onActionSave()
{
    if(!_model->isImageDataLoaded())
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
    if(_model->isImageDataLoaded())
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
