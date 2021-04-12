#ifndef VIDEOLOADER_H
#define VIDEOLOADER_H
#include <QObject>
#include <QList>
#include <QImage>
#include <QDebug>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <string>

using namespace cv;
using namespace std;

class VideoLoader
{
public:
    explicit VideoLoader(QObject *parent = nullptr);
    QImage getImageVideoAt(int index);
    void loadVideo(string fileName);

signals:

private:
    QList<QImage> frameList;

public:
    int getSize();
};

#endif // VIDEOLOADER_H
