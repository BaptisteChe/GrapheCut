#include "videoloader.h"

VideoLoader::VideoLoader(QObject *parent)
{
    this->isVideoTreated = false;
}

QImage VideoLoader::getImageVideoAt(int index)
{
    if(this->isVideoTreated)
    {
        return this->frameListResult.at(index);
    }
    else
    {
        return this->frameList.at(index);
    }
}

QList<QImage> VideoLoader::getFrameList()
{
    return this->frameList;
}

void VideoLoader::setFrameListResult(QList<QImage> listResult)
{
    this->frameListResult = listResult;
}

void VideoLoader::loadVideo(string fileName)
{
    Mat frame;
    VideoCapture videoCapture;
    videoCapture.open(fileName, cv::CAP_FFMPEG);
    if (!videoCapture.isOpened()) {
            qDebug() << "ERROR! Unable to open camera\n";
    }
    else
    {
        //lecture de la video
        for(;;)
        {
            videoCapture.read(frame);

            //si on trouve une frame vide on arrete la lecture
            if(frame.empty())
            {
                break;
            }

            //creation QImage a partie de l image chargee
            QImage image ((const uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

            frameList.append(image.rgbSwapped());
        }
    }
}

void VideoLoader::createVideo(String videoName)
{

    VideoWriter video(videoName, cv::VideoWriter::fourcc('M','J','P','G'), 30, Size(this->frameListResult.at(0).width(),this->frameListResult.at(0).height()));

    int i = 0;
    for(;;)
    {
        if(i == this->frameListResult.size() - 1)
        {
            break;
        }

        //conversion image en matrice
        QImage image = this->frameListResult.at(i).rgbSwapped();
        Mat frame(image.height(), image.width(), CV_8UC3, image.bits(), image.bytesPerLine());

        //ecriture de la video
        video.write(frame);

        i++;
    }
}

void VideoLoader::setIsViedoTreated(bool isTreated)
{
    this->isVideoTreated = isTreated;
}


int VideoLoader::getSize()
{
    return this->frameList.size();
}
