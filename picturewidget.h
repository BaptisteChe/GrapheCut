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
    void loadImage(QImage im);
    void saveImage(QString imageName);
    void addLayer();
    void clearLayer();
    void setDrawInProgress(bool value);
    void setColorDraw(bool blue, bool red);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void drawMouse(int lastPosX, int lastPosY, int newPosX, int newPosY, bool isLine);
    bool isLayerValid();
    void setIsFirstFrame(bool isFirstFrame);

signals:


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage image;
    QImage layer;
    int posMouseX;
    int posMouseY;
    bool mousePress;        // clic long de souris
    bool drawInProgress;    // dessin en cours
    bool redDraw;           // dessin rouge en cours
    bool blueDraw;          // dessin bleu en cours
    bool isLayerBlue;       // le calque comporte du bleu dans ses pixels
    bool isLayerRed;        // le calque comporte du rouge dans ses pixels
    bool isFirstFrame;
    QPen pen;
};



#endif // PICTUREWIDGET_H
