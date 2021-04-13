#ifndef PICTUREWIDGET_H
#define PICTUREWIDGET_H

#include <QWidget>
#include <QImage>
#include <QProgressBar>
#include <QMouseEvent>
#include <QPainter>


class PictureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PictureWidget(QWidget *parent = nullptr);
    void loadImage(QImage im, int index, int tailleListeFrame, QProgressBar *progressBar);
    void setDrawInProgress(bool value);
    void setColorDraw(bool blue, bool red);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);


signals:


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage image;
    int posMouseX;
    int posMouseY;
    bool mousePresse;
    bool drawInProgress;
    bool redDraw;
    bool blueDraw;
    QPen pen;
};



#endif // PICTUREWIDGET_H
