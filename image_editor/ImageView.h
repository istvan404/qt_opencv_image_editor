#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QMainWindow>
#include <QFileDialog>

#include <QDebug>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class ImageView : public QMainWindow
{
    Q_OBJECT

public:
    ImageView(QWidget *parent = nullptr);
    ~ImageView();

private:

    // Menu
    QMenuBar* _menuBar;

    // File
    QMenu* _fileMenu;
    QAction* _actionLoad;
    QAction* _actionSave;
    QAction* _actionExit;

private slots:
    void onLoadAction();
    void onSaveAction();
    void onExitAction();
};
#endif // IMAGEVIEW_H
