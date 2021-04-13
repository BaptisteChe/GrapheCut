#include "picturewidget.h"
#include <QPainter>
#include <QDebug>


PictureWidget::PictureWidget(QWidget *parent) : QWidget(parent)
{
    this->drawInProgress = false;
    this->posMouseX = 0;
    this->posMouseY = 0;
    this->mousePresse = false;
    this->pen.setWidth(10);

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

void PictureWidget::setDrawInProgress(bool value)
{
    this->drawInProgress = value;
}

void PictureWidget::setColorDraw(bool blue, bool red)
{
    this->blueDraw = blue;
    this->redDraw = red;
}

void PictureWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(this->drawInProgress && this->mousePresse)
    {
        this->posMouseX = event->pos().x();
        this->posMouseY = event->pos().y();
        update();
    }
}

void PictureWidget::mousePressEvent(QMouseEvent *event)
{
    if(this->drawInProgress)
    {
        this->mousePresse = true;
        this->posMouseX = event->pos().x();
        this->posMouseY = event->pos().y();
        update();
    }
}

void PictureWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    QPainter painterImage(&this->image);
    if(this->drawInProgress)
    {
        if(this->blueDraw)
        {
            this->pen.setColor(Qt::blue);
        }
        else if(this->redDraw)
        {
            this->pen.setColor(Qt::red);
        }
        painterImage.setPen(pen);
        int positionXImage = this->posMouseX * this->image.size().width() / this->size().width();
        int positionYImage = this->posMouseY * this->image.size().height() / this->size().height();
        painterImage.drawPoint(positionXImage, positionYImage);
        qDebug()<<"dessin point";
        painterImage.end();
    }
    painter.drawImage( this->rect(), this->image);
}



