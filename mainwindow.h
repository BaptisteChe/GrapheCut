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
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include "imagegraphtreatment.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonImageLoading_clicked();

    void on_buttonVideoLoading_clicked();

    void on_buttonSave_clicked();

    void on_buttonForegroundDrawing_clicked();

    void on_buttonBackgroundDrawing_clicked();

    void on_buttonTreatment_clicked();

    void on_buttonClear_clicked();

    void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    bool isButtonBlueSelected;
    bool isButtonRedSelected;
    bool imageTreatment;
    bool videoTreatment;
    VideoLoader *videoLoader;
    QString fileName;
    ImageGraphTreatment *imageGraphTreatment;
};
#endif // MAINWINDOW_H
