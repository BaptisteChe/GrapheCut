#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QStringList>
#include <QtMultimedia>
#include <QUrl>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonImageLoading_clicked()
{
    imageTreatment = true;
    videoTreatment = false;

    blueDraw = false;
    redDraw = false;
    buttonBlueDraw = false;
    buttonRedDraw = false;
    ui->buttonPlay->setEnabled(false);
    ui->buttonReload->setEnabled(false);
    ui->buttonSave->setEnabled(false);
    ui->buttonStop->setEnabled(false);
    ui->buttonTreatment->setEnabled(false);

    QStringList fileNames;
    QFileDialog *fileDialog = new QFileDialog();

    if (fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
        QImage image;
        if(image.load(fileNames.at(0)))
        {
            ui->pictureWidget->loadImage(image);
            ui->buttonBackgroundDrawing->setEnabled(true);
            ui->buttonBackgroundDrawing->setText("Bleu");
            ui->buttonForegroundDrawing->setEnabled(true);
            ui->buttonForegroundDrawing->setText("Rouge");
        }
        else
        {
            qDebug() << "fail";
        }
    }
}

void MainWindow::on_buttonVideoLoading_clicked()
{
    videoTreatment = true;
    imageTreatment = false;

    blueDraw = false;
    redDraw = false;
    buttonBlueDraw = false;
    buttonRedDraw = false;
    ui->buttonPlay->setEnabled(false);
    ui->buttonReload->setEnabled(false);
    ui->buttonSave->setEnabled(false);
    ui->buttonStop->setEnabled(false);
    ui->buttonTreatment->setEnabled(false);

    QStringList fileNames;
    QFileDialog *fileDialog = new QFileDialog();
    fileDialog->show();
    if (fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
        qDebug() <<fileNames.at(0);

        QMediaPlayer *mediaPlayer = new QMediaPlayer;
        QMediaPlaylist *playlist = new QMediaPlaylist(mediaPlayer);
        this->videoLoader = new VideoLoader;
        playlist->addMedia(QUrl::fromLocalFile(fileNames.at(0)));

        mediaPlayer->setVideoOutput(videoLoader);
        playlist->setCurrentIndex(1);
        mediaPlayer->setPlaylist(playlist);
        mediaPlayer->play();

//#warning pb parce que la video a pas le temps d etre decodee et donc la liste de frame n est pas remplie, trouver un moyen de faire attendre avant de pouvoir afficher
        //ui->pictureWidget->loadImage(videoLoader->getImageVideoAt(1));

        ui->buttonBackgroundDrawing->setEnabled(true);
        ui->buttonBackgroundDrawing->setText("Bleu");
        ui->buttonForegroundDrawing->setEnabled(true);
        ui->buttonForegroundDrawing->setText("Rouge");
    }
}

void MainWindow::on_buttonSave_clicked()
{

}

void MainWindow::on_buttonForegroundDrawing_clicked()
{
    if(!buttonRedDraw)
    {
        ui->buttonBackgroundDrawing->setEnabled(false);
        ui->buttonForegroundDrawing->setText("Valider Rouge");
        buttonRedDraw = true;
    }
    else
    {
        ui->buttonBackgroundDrawing->setEnabled(true);
        ui->buttonForegroundDrawing->setText("Rouge");
        buttonRedDraw = false;

        if(redDraw && blueDraw)
        {
            ui->buttonTreatment->setEnabled(true);
        }
    }
}

void MainWindow::on_buttonBackgroundDrawing_clicked()
{
    if(!buttonBlueDraw)
    {
        ui->buttonForegroundDrawing->setEnabled(false);
        ui->buttonBackgroundDrawing->setText("Valider Bleu");
        buttonBlueDraw = true;
    }
    else
    {
        ui->buttonForegroundDrawing->setEnabled(true);
        ui->buttonBackgroundDrawing->setText("Bleu");
        buttonBlueDraw = false;

        if(redDraw && blueDraw)
        {
            ui->buttonTreatment->setEnabled(true);
        }
    }
}

void MainWindow::on_buttonTreatment_clicked()
{
    buttonRedDraw = false;
    buttonBlueDraw = false;
    ui->buttonBackgroundDrawing->setEnabled(false);
    ui->buttonForegroundDrawing->setEnabled(false);
    ui->buttonTreatment->setEnabled(false);

    if(imageTreatment)
    {

    }
    else if(videoTreatment)
    {
        ui->buttonPlay->setEnabled(true);
        ui->buttonReload->setEnabled(true);
        ui->buttonStop->setEnabled(true);
    }
}

