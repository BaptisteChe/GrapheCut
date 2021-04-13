#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputEvent>
#include <QMouseEvent>
#include <QFileDialog>
#include <QStringList>
#include <QtMultimedia>
#include <QUrl>
#include <QDebug>
#include "videoloader.h"
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/videoio.hpp>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
//    void mouseMoveEvent(QMouseEvent *event);
//    void mousePressEvent(QMouseEvent *event);
    ~MainWindow();

private slots:
    void on_buttonImageLoading_clicked();

    void on_buttonVideoLoading_clicked();

    void on_buttonSave_clicked();

    void on_buttonForegroundDrawing_clicked();

    void on_buttonBackgroundDrawing_clicked();

    void on_buttonTreatment_clicked();

private:
    Ui::MainWindow *ui;
    bool buttonBlueDraw;
    bool buttonRedDraw;
    bool redDraw;
    bool blueDraw;
    bool valideDraw;
    bool imageTreatment;
    bool videoTreatment;
    VideoLoader *videoLoader;
};
#endif // MAINWINDOW_H
