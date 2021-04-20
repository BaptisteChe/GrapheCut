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

void PictureWidget::loadImage(QImage im, int index, int tailleListeFrame, QSlider *slider)
{
    if(index != -1)
    {
        int pourcentageVideo = (int)(99 * index / tailleListeFrame);
        slider->setValue(pourcentageVideo);
    }
    this->image = im;
    update();
}

void PictureWidget::addLayer()
{
    QImage im(this->image.width(), this->image.height(),QImage::Format_RGBA8888);
    this->layer = im;
    this->layer.fill(qRgba(0, 0, 0, 0));
    update();
}

void PictureWidget::clearLayer()
{
    this->layer.fill(qRgba(0, 0, 0, 0));
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
        int lastPosX = this->posMouseX;
        int lastPosY = this->posMouseY;
        this->posMouseX = event->pos().x() * this->image.size().width() / this->size().width();
        this->posMouseY = event->pos().y() * this->image.size().height() / this->size().height();
        drawMouse(lastPosX,lastPosY,this->posMouseX,this->posMouseY,true);
        update();
    }
}

void PictureWidget::mousePressEvent(QMouseEvent *event)
{
    if(this->drawInProgress)
    {
        this->mousePresse = true;
        this->posMouseX = event->pos().x() * this->image.size().width() / this->size().width();
        this->posMouseY = event->pos().y() * this->image.size().height() / this->size().height();
        drawMouse(0,0,this->posMouseX,this->posMouseY,false);
        update();
    }
}

void PictureWidget::drawMouse(int lastPosX, int lastPosY, int newPosX, int newPosY, bool isLine)
{
    QPainter painterLayer(&this->layer);
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
        painterLayer.setPen(pen);
        if(isLine)
        {
            painterLayer.drawLine(lastPosX,lastPosY,newPosX,newPosY);
        }
        else
        {
            painterLayer.drawPoint(newPosX, newPosY);
        }
        painterLayer.end();
    }
}

void PictureWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.drawImage( this->rect(), this->image);
    painter.drawImage(this->rect(), this->layer);
}



