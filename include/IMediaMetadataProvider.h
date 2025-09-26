#pragma once
#include <QSize>
#include <QString>

struct VideoInfo {
  QString filePath;
  QString mimeType; // "video/mp4" or "image/jpeg" https://doc.qt.io/qt-6/qtcore-mimetypes-mimetypebrowser-example.html
  QSize   resolution;
  QString videoCodec;
  QString audioCodec;
  QString orientation;
  qint64  fileBytes    = 0;
  qint64  durationMs   = 0;
  double  frameRate    = 0.0;
  int     videoBitRate = 0;
  int     audioBitRate = 0;
};

class IMediaMetadataProvider {
public:
  virtual ~IMediaMetadataProvider() = default;

  // Extract metadata from 'filePath'
  // meta data for the file will be saved into 'videoInfo'
  // On failure returns false and sets 'errorMessage'
  virtual bool extract(const QString &filePath, VideoInfo &videoInfo, QString &errorMessage) = 0;
};
