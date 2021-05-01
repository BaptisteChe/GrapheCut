#include "picturewidget.h"
#include <QPainter>
#include <QDebug>


PictureWidget::PictureWidget(QWidget *parent) : QWidget(parent)
{
    this->drawInProgress = false;
    this->posMouseX = 0;
    this->posMouseY = 0;
    this->mousePress = false;
    this->pen.setWidth(10);
    this->isLayerBlue = false;
    this->isLayerRed = false;
    this->blueDraw = false;
    this->redDraw = false;
    this->isFirstFrame =false;
}

void PictureWidget::loadImage(QImage im)
{
    this->image = im;
    update();
}

void PictureWidget::saveImage(QString imageName)
{
    this->image.save(imageName);
}

void PictureWidget::addLayer()
{
    QImage im(this->image.width(), this->image.height(),QImage::Format_RGBA8888);
    this->layer = im;
    this->layer.fill(qRgba(0, 0, 0, 0));
    this->isLayerBlue = false;
    this->isLayerRed = false;
    update();
}

QImage PictureWidget::getLayer()
{
    return this->layer;
}

void PictureWidget::clearLayer()
{
    this->isLayerRed = false;
    this->isLayerBlue = false;
    this->layer.fill(qRgba(0, 0, 0, 0));    //calque remplis de pixels transparents
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
    if(this->drawInProgress && this->mousePress)
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
        this->mousePress = true;
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
            this->isLayerBlue = true;
        }
        else if(this->redDraw)
        {
            this->pen.setColor(Qt::red);
            this->isLayerRed = true;
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

bool PictureWidget::isLayerValid()
{
    return (this->isLayerRed && this->isLayerBlue) ?  true : false;
}

void PictureWidget::setIsFirstFrame(bool isFirstFrame)
{
    this->isFirstFrame = isFirstFrame;
}

void PictureWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.drawImage( this->rect(), this->image);
    if(this->isFirstFrame)
    {
        painter.drawImage(this->rect(), this->layer);
    }
}



