#include "videoloader.h"
#include <QMultimedia>
#include <QDebug>

VideoLoader::VideoLoader(QObject *parent) : QAbstractVideoSurface(parent)
{

}

QImage VideoLoader::getImageVideoAt(int index)
{
    return frameList.at(index);
}



QList<QVideoFrame::PixelFormat> VideoLoader::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
{
    QList<QVideoFrame::PixelFormat> formatList;
    formatList.append(QVideoFrame::Format_RGB24);
    return formatList;
}

bool VideoLoader::present(const QVideoFrame &frame)
{
    QImage::Format imageFormat = QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
    QImage image( frame.bits(), frame.width(), frame.height(), frame.bytesPerLine(), imageFormat);
    qDebug()<<"image : "<<frame.bits();
    frameList.append(image);

    return true;
}

int VideoLoader::getSize()
{
    return this->frameList.size();
}
