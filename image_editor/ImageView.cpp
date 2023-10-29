#include "ImageView.h"

class AdjustmentGridLayout : public QGridLayout
{
public:
    AdjustmentGridLayout(QString title, int min, int max, int defaultValue)
    {
        _title = title;
        _min = min;
        _max = max;
        _defaultValue = defaultValue;

        _labelTitle = new QLabel(_title);
        _labelMin = new QLabel(QString::number(_min));
        _labelMax = new QLabel(QString::number(_max));
        _labelValue = new QLabel(QString::number(_defaultValue));
        _slider = new QSlider(Qt::Horizontal);
        _button = new QPushButton("Apply");

        _slider->setRange(_min, _max);
        _slider->setTickInterval(1);
        _slider->setValue(_defaultValue);
        _slider->setTickPosition(QSlider::TicksAbove);

        this->setSpacing(0);
        this->addWidget(_labelTitle, 0, 0, 1, 4);
        this->addWidget(_labelMin, 1, 0, 1, 1);
        this->addWidget(_labelMax, 1, 2, 1, 1, Qt::AlignRight);
        this->addWidget(_labelValue, 2, 3, 1, 1, Qt::AlignCenter);
        this->addWidget(_slider, 2, 0, 1, 3);
        this->addWidget(_button, 3, 0, 1, 4);

        connect(_slider, &QSlider::valueChanged, this, &AdjustmentGridLayout::onSliderValueChanged);
    }

    QPushButton* button()
    {
        return _button;
    }
private:
    QString _title;
    int _min;
    int _max;
    int _defaultValue;

    QLabel* _labelTitle;
    QLabel* _labelMin;
    QLabel* _labelMax;
    QLabel* _labelValue;
    QSlider* _slider;
    QPushButton* _button;

private slots:
    void onSliderValueChanged();
};

void AdjustmentGridLayout::onSliderValueChanged()
{
    _labelValue->setText( QString::number(_slider->value()) );
}


ImageView::ImageView(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Image Editor - OpenCV 4.8.1");
    setFixedSize(1280, 720);
    qDebug() << "OpenCV current version: " << CV_VERSION;

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
    _layoutAdjustments =        new QVBoxLayout();
    _labelAdjustmentsTitle =    new QLabel("Adjustments");
    _buttonAdjustmentsReset =   new QPushButton("Reset");

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

    // Connect SIGNALs to SLOTs
    connect(_buttonAdjustmentsReset, SIGNAL(clicked(bool)), this, SLOT(onAdjustmentsResetButtonClicked()));

    // Testing new design
    _layoutAdjustmentsWhiteBalance = new QGridLayout();
    _labelAdjustmentWhiteBalance = new QLabel("White Balance");
    _sliderAdjustmentWhiteBalanceSlider = new QSlider(Qt::Orientation::Horizontal);
    _labelAdjustmentWhiteBalanceMin = new QLabel("0");
    _labelAdjustmentWhiteBalanceMax = new QLabel("20");
    _labelAdjustmentWhiteBalanceValue = new QLabel("0");
    _buttonAdjustmentWhiteBalanceButton = new QPushButton("Apply");

    _sliderAdjustmentWhiteBalanceSlider->setRange(0, 20);
    _sliderAdjustmentWhiteBalanceSlider->setTickInterval(1);
    _sliderAdjustmentWhiteBalanceSlider->setValue(0);
    _sliderAdjustmentWhiteBalanceSlider->setTickPosition(QSlider::TicksAbove);

    QFont font = QFont();
    font.setPointSize(10);
    _labelAdjustmentWhiteBalance->setFont(font);

    _layoutAdjustmentsWhiteBalance->setSpacing(0);
    _layoutAdjustmentsWhiteBalance->addWidget(_labelAdjustmentWhiteBalance, 0, 0, 1, 4);
    _layoutAdjustmentsWhiteBalance->addWidget(_labelAdjustmentWhiteBalanceMin, 1, 0, 1, 1);
    _layoutAdjustmentsWhiteBalance->addWidget(_labelAdjustmentWhiteBalanceMax, 1, 2, 1, 1, Qt::AlignRight);
    _layoutAdjustmentsWhiteBalance->addWidget(_sliderAdjustmentWhiteBalanceSlider, 2, 0, 1, 3);
    _layoutAdjustmentsWhiteBalance->addWidget(_labelAdjustmentWhiteBalanceValue, 2, 3, 1, 1, Qt::AlignCenter);
    _layoutAdjustmentsWhiteBalance->addWidget(_buttonAdjustmentWhiteBalanceButton, 3, 0, 1, 4);
    _layoutAdjustments->addLayout(_layoutAdjustmentsWhiteBalance);

    connect(_sliderAdjustmentWhiteBalanceSlider, SIGNAL(valueChanged(int)), this, SLOT(onAdjustmentWhiteBalanceSliderReleased()));
    connect(_buttonAdjustmentWhiteBalanceButton, SIGNAL(clicked(bool)), this, SLOT(onAdjustmentWhiteBalanceButtonClicked()));

    // Testing out custom UI element
    AdjustmentGridLayout* shadowAdjustment = new AdjustmentGridLayout("Shadow",
                                                                      0,
                                                                      20,
                                                                      0);
    _layoutAdjustments->addLayout(shadowAdjustment);

    connect(shadowAdjustment->button(), SIGNAL(clicked(bool)), this, SLOT(onAdjustmentWhiteBalanceButtonClicked()));

}

void ImageView::onAdjustmentsResetButtonClicked()
{
    if(!_model->isImageLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }

    _model->editReset();
}

void ImageView::onAdjustmentWhiteBalanceSliderReleased()
{
    _labelAdjustmentWhiteBalanceValue->setText( QString::number(_sliderAdjustmentWhiteBalanceSlider->value()) );
}

void ImageView::onAdjustmentWhiteBalanceButtonClicked()
{
    if(!_model->isImageLoaded())
    {
        QMessageBox::warning(this,
                             "Image Editor - Warning",
                             "There is no image loaded");
        return;
    }

    this->setCursor(Qt::CursorShape::BusyCursor);
    _model->editAutoWhiteBalance(_sliderAdjustmentWhiteBalanceSlider->value());
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
    _histogramGraphicsView->fitInView(_histogramGraphicsScene->sceneRect(), Qt::KeepAspectRatio);
}

void ImageView::onImageModelUpdated()
{
    qDebug() << "View catched model's ImageUpdated signal" << QTime::currentTime();
    loadImage();
}

void ImageView::onActionLoad()
{
    if(_model->isImageLoaded())
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
