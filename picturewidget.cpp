#include "picturewidget.h"
#include <QPainter>
#include <QDebug>


PictureWidget::PictureWidget(QWidget *parent) : QWidget(parent)
{

}

void PictureWidget::loadImage(QImage im)
{
    qDebug()<<"image : "<<im.width();
    this->image = im;
    update();
}

void PictureWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.drawImage( this->rect(), this->image);
}



