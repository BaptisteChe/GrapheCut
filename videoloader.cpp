#include "videoloader.h"

VideoLoader::VideoLoader(QObject *parent)
{

}

QImage VideoLoader::getImageVideoAt(int index)
{
    return frameList.at(index);
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


int VideoLoader::getSize()
{
    return this->frameList.size();
}
