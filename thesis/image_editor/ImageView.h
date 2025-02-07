#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include "ImageModel.h"
#include "Adjustment.h"
#include <QMainWindow>
#include <QMessageBox>
#include <QMenuBar>
#include <QFileDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCursor>
#include <QSlider>
#include <QMouseEvent>

#include <QGraphicsScene>
#include <QGraphicsView>

class ImageView : public QMainWindow
{
    Q_OBJECT

public:
    ImageView(QWidget *parent = nullptr);
    ~ImageView();

private:
    ImageModel* _model;

    // Menu
    QMenuBar* _menuBar;

    // Menu: File
    QMenu* _menuFile;
    QAction* _actionLoad;
    QAction* _actionSave;
    QAction* _actionExit;

    // Menu: File
    QMenu* _menuView;
    QAction* _actionZoomIn;
    QAction* _actionZoomOut;
    QAction* _actionZoomFit;
    QAction* _actionFlipHorizontal;
    QAction* _actionFlipVertical;
    QAction* _actionRotate90CW;
    QAction* _actionRotate90CCW;
    QAction* _actionRotate180;

    // Layout
    QWidget* _centralWidget;
    QHBoxLayout* _layoutMain;

    // Layout: Image
    QVBoxLayout* _layoutImageContainer;
    QGraphicsScene* _imageGraphicsScene;
    QGraphicsView* _imageGraphicsView;

    // Layout: Side
    QVBoxLayout* _layoutSide;
    // Layout: Side: Adjustments
    QVBoxLayout* _layoutAdjustments;
    QLabel* _labelAdjustmentsTitle;
    QPushButton* _buttonAdjustmentsReset;
    // Layout: Side: Adjustments: White Balance
    AdjustmentSlider* _adjustmentWhiteBalance;
    AdjustmentButton* _adjustmentWhiteBalanceGW;
    AdjustmentSlider* _adjustmentBrightness;
    AdjustmentSlider* _adjustmentShadowBasic;
    AdjustmentSlider* _adjustmentShadow;

    // Layout: Side: Histogram
    QVBoxLayout* _layoutHistogram;
    QGraphicsScene* _histogramGraphicsScene;
    QGraphicsView* _histogramGraphicsView;

    void setupCentralWidget();
    void setupMenuBar();
    void setupMenuFile();
    void setupMenuView();
    void setupImageViewport();
    void setupSide();
    void setupAdjustments();
    void setupHistogram();
    void connectModel();
    void loadImage();

private slots:
    // SLOTs of model's signals
    void onImageModelLoaded();
    void onImageModelUpdated();
    void onImageModelLoadError();
    void onImageModelSaveError();

    // Menu Actions
    void onActionLoad();
    void onActionSave();
    void onActionExit();
    void onActionZoomIn();
    void onActionZoomOut();
    void onActionZoomFit();
    void onActionFlipHorizontal();
    void onActionFlipVertical();
    void onActionRotate90CW();
    void onActionRotate90CCW();
    void onActionRotate180();

    // SLOTs of adjustments
    void onAdjustmentsResetButtonClicked();
    void onAdjustmentWhiteBalanceClicked();
    void onAdjustmentWhiteBalanceGWClicked();
    void onAdjustmentBrightnessClicked();
    void onAdjustmentShadowBasicClicked();
    void onAdjustmentShadowClicked();
};
#endif // IMAGEVIEW_H
