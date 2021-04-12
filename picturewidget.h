#ifndef PICTUREWIDGET_H
#define PICTUREWIDGET_H

#include <QWidget>
#include <QImage>

class PictureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PictureWidget(QWidget *parent = nullptr);
    void loadImage(QImage im);

signals:


    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage image;
};



#endif // PICTUREWIDGET_H
