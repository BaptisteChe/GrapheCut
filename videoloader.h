#ifndef VIDEOLOADER_H
#define VIDEOLOADER_H
#include <QObject>
#include <QAbstractVideoSurface>
#include <QList>
#include <QVideoFrame>
#include <QImage>
/*#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>*/

class VideoLoader : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit VideoLoader(QObject *parent = nullptr);
    QImage getImageVideoAt(int index);

signals:

private:
    QList<QImage> frameList;


    // QAbstractVideoSurface interface
public:
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const override;
    bool present(const QVideoFrame &frame) override;
    int getSize();
};

#endif // VIDEOLOADER_H
