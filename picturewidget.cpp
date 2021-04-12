#include "picturewidget.h"
#include <QPainter>
#include <QDebug>


PictureWidget::PictureWidget(QWidget *parent) : QWidget(parent)
{

}

void PictureWidget::loadImage(QImage im, int index, int tailleListeFrame, QProgressBar *progressBar)
{
    if(index != -1)
    {
        int pourcentageVideo = (int)(100 * index / tailleListeFrame);
        progressBar->setValue(pourcentageVideo);
    }
    this->image = im;
    update();
}

void PictureWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.drawImage( this->rect(), this->image);
}



