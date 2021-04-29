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

void VideoLoader::createVideo(String videoName)
{

    VideoWriter video(videoName, cv::VideoWriter::fourcc('M','J','P','G'), 30, Size(this->frameList.at(0).width(),this->frameList.at(0).height()));

    int i = 0;
    for(;;)
    {
        if(i == this->frameList.size() - 1)
        {
            break;
        }

        //conversion image en matrice
        QImage image = this->frameList.at(i);
        Mat frame(image.height(), image.width(), CV_8UC3, image.bits(), image.bytesPerLine());

        //ecriture de la video
        video.write(frame);

        i++;
    }
}


int VideoLoader::getSize()
{
    return this->frameList.size();
}
