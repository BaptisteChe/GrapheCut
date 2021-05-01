#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->isButtonBlueSelected = false;
    this->isButtonRedSelected = false;
    this->imageTreatment = false;
    this->videoTreatment = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonImageLoading_clicked()
{
    imageTreatment = true;
    videoTreatment = false;

    isButtonBlueSelected = false;
    isButtonRedSelected = false;
    ui->buttonSave->setEnabled(false);
    ui->buttonTreatment->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);
    this->videoLoader->setIsViedoTreated(false);

    QStringList fileNames;
    QFileDialog *fileDialog = new QFileDialog();

    if (fileDialog->exec())
    {
        //recuperation chemin du fichier selectionne
        fileNames = fileDialog->selectedFiles();
        QImage image;
        this->fileName = fileNames.at(0);

        // Si l image peut etre chargee
        if(image.load(this->fileName))
        {
            //recuperation et affichage de l image
            ui->pictureWidget->loadImage(image);
            ui->pictureWidget->addLayer();
            ui->pictureWidget->setIsFirstFrame(true);

            ui->pictureWidget->setColorDraw(false,false);
            ui->buttonBackgroundDrawing->setEnabled(true);
            ui->buttonBackgroundDrawing->setText("Bleu");
            ui->buttonForegroundDrawing->setEnabled(true);
            ui->buttonForegroundDrawing->setText("Rouge");
            ui->buttonClear->setEnabled(true);

            ui->labelInformations->setText(QLatin1String("Informations : Image de taille ") + QString::number(image.width()) + QLatin1String("x") + QString::number(image.height()));
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

    isButtonBlueSelected = false;
    isButtonRedSelected = false;
    ui->buttonSave->setEnabled(false);
    ui->buttonTreatment->setEnabled(false);
    ui->horizontalSlider->setEnabled(false);
    this->videoLoader->setIsViedoTreated(false);

    QStringList fileNames;
    QFileDialog *fileDialog = new QFileDialog();
    fileDialog->show();
    if (fileDialog->exec())
    {
        //recuperation chemin du fichier selectionne
        fileNames = fileDialog->selectedFiles();
        this->fileName = fileNames.at(0);

        this->videoLoader = new VideoLoader;
        this->videoLoader->loadVideo(this->fileName.toUtf8().constData());

        //recuperation et affichage de la premiere image de la video
        ui->pictureWidget->loadImage(videoLoader->getImageVideoAt(0));
        ui->pictureWidget->addLayer();
        ui->pictureWidget->setIsFirstFrame(true);

        ui->pictureWidget->setColorDraw(false,false);
        ui->buttonBackgroundDrawing->setEnabled(true);
        ui->buttonBackgroundDrawing->setText("Bleu");
        ui->buttonForegroundDrawing->setEnabled(true);
        ui->buttonForegroundDrawing->setText("Rouge");
        ui->buttonClear->setEnabled(true);

        QImage image = this->videoLoader->getImageVideoAt(0);
        ui->labelInformations->setText(QLatin1String("Informations : Image de taille ") + QString::number(image.width()) + QLatin1String("x") + QString::number(image.height())
                                       + ", Frame : " + QString::number(this->videoLoader->getSize()));
    }
}

void MainWindow::on_buttonSave_clicked()
{
    //recuperation du nom du fichier choisit par l utilisateur
    QStringList listPath;
    listPath = this->fileName.split(QDir::separator());

    QString fileNameWithExtension = listPath.last();
    QStringList listFileName = fileNameWithExtension.split(".");

    this->fileName = listFileName.first();

    if(this->imageTreatment)
    {
        //on garde l extension pour pouvoir sauvegarder l image traitee avec le meme format que l image initiale
        this->fileName += QLatin1String("_traitee.") + listFileName.last();
        ui->pictureWidget->saveImage(this->fileName);
    }
    else
    {
        String fileNameWithoutExtension = this->fileName.toUtf8().constData();
        fileNameWithoutExtension += "_traitee.avi";
        this->videoLoader->createVideo(fileNameWithoutExtension);
    }
}

void MainWindow::on_buttonForegroundDrawing_clicked()
{
    if(!isButtonRedSelected)
    {
        ui->buttonBackgroundDrawing->setEnabled(false);
        ui->buttonForegroundDrawing->setText("Valider Rouge");

        isButtonRedSelected = true;
        ui->pictureWidget->setDrawInProgress(true);
        ui->pictureWidget->setColorDraw(false,true);
    }
    else
    {
        ui->buttonBackgroundDrawing->setEnabled(true);
        ui->buttonForegroundDrawing->setText("Rouge");

        isButtonRedSelected = false;
        ui->pictureWidget->setDrawInProgress(false);
        ui->pictureWidget->setColorDraw(false,false);

        if(ui->pictureWidget->isLayerValid())
        {
            ui->buttonTreatment->setEnabled(true);
        }
    }
}

void MainWindow::on_buttonBackgroundDrawing_clicked()
{
    if(!isButtonBlueSelected)
    {
        ui->buttonForegroundDrawing->setEnabled(false);
        ui->buttonBackgroundDrawing->setText("Valider Bleu");

        isButtonBlueSelected = true;
        ui->pictureWidget->setDrawInProgress(true);
        ui->pictureWidget->setColorDraw(true,false);
    }
    else
    {
        ui->buttonForegroundDrawing->setEnabled(true);
        ui->buttonBackgroundDrawing->setText("Bleu");

        isButtonBlueSelected = false;
        ui->pictureWidget->setDrawInProgress(false);
        ui->pictureWidget->setColorDraw(false,false);

        if(ui->pictureWidget->isLayerValid())
        {
            ui->buttonTreatment->setEnabled(true);
        }
    }
}

void MainWindow::on_buttonTreatment_clicked()
{
    isButtonRedSelected = false;
    isButtonBlueSelected = false;
    ui->buttonBackgroundDrawing->setEnabled(false);
    ui->buttonForegroundDrawing->setEnabled(false);
    ui->buttonClear->setEnabled(false);
    ui->buttonTreatment->setEnabled(false);

    this->imageGraphTreatment = new ImageGraphTreatment;

    //traitement de l image
    if(imageTreatment)
    {
        QImage image = this->imageGraphTreatment->InitTraitementImage(ui->pictureWidget->getImage() , ui->pictureWidget->getLayer());
        ui->pictureWidget->loadImage(image);
    }
    //traitement de la video
    else if(videoTreatment)
    {
        QList<QImage> resultListe = this->imageGraphTreatment->traitementVideo(this->videoLoader->getFrameList(), ui->pictureWidget->getLayer());
        this->videoLoader->setFrameListResult(resultListe);
        ui->horizontalSlider->setEnabled(true);
        this->videoLoader->setIsViedoTreated(true);
        ui->pictureWidget->loadImage(this->videoLoader->getImageVideoAt(0));
    }

    ui->buttonSave->setEnabled(true);
}

void MainWindow::on_buttonClear_clicked()
{
    ui->pictureWidget->clearLayer();
    ui->buttonTreatment->setEnabled(false);
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    //calcul de la frame a afficher
    int frameShow = (int)((this->videoLoader->getSize() - 1) * position / ui->horizontalSlider->maximum());

    //affichage
    ui->pictureWidget->loadImage(this->videoLoader->getImageVideoAt(frameShow));
    if(position == 0)
    {
        ui->pictureWidget->setIsFirstFrame(true);
    }
    else
    {
        ui->pictureWidget->setIsFirstFrame(false);
    }
    QImage image = this->videoLoader->getImageVideoAt(frameShow);
    ui->labelInformations->setText(QLatin1String("Informations : Image de taille ") + QString::number(image.width()) + QLatin1String("x") + QString::number(image.height())
                                   + ", Frame : " + QString::number(frameShow + 1));
}
