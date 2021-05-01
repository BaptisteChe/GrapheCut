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
    QList<QImage> getFrameList();
    void setFrameListResult(QList<QImage> resultList);
    void loadVideo(string fileName);
    void createVideo(String videoName);
    void setIsViedoTreated(bool isTreated);

signals:

private:
    QList<QImage> frameList;
    QList<QImage> frameListResult;
    bool isVideoTreated;

public:
    int getSize();
};

#endif // VIDEOLOADER_H
