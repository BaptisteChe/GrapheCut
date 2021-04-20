#ifndef PICTUREWIDGET_H
#define PICTUREWIDGET_H

#include <QWidget>
#include <QImage>
#include <QSlider>
#include <QMouseEvent>
#include <QPainter>


class PictureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PictureWidget(QWidget *parent = nullptr);
    void loadImage(QImage im, int index, int tailleListeFrame, QSlider *progressBar);
    void addLayer();
    void clearLayer();
    void setDrawInProgress(bool value);
    void setColorDraw(bool blue, bool red);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void drawMouse(int lastPosX, int lastPosY, int newPosX, int newPosY, bool isLine);

signals:


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage image;
    QImage layer;
    int posMouseX;
    int posMouseY;
    bool mousePresse;
    bool drawInProgress;
    bool redDraw;
    bool blueDraw;
    QPen pen;
};



#endif // PICTUREWIDGET_H
