#include "Video.h"

#include <QFileInfo>
#include <QMimeDatabase>
#include <QMimeType>
#include <QTextStream>
#include <stdexcept>

Video::Video(QString path, std::unique_ptr<IMediaMetadataProvider> provider)
    : filePath(std::move(path)), mediaMetadataprovider(std::move(provider)) {
  if (!mediaMetadataprovider) {
    throw std::runtime_error("Video: metadata provider is null.");
  }

  checkVideoFilePath(filePath);
  validateMIMEtype(filePath);

  QString err;
  if (!mediaMetadataprovider->extract(filePath, videoInfo, err)) {
    throw std::runtime_error(QString("Failed to extract metadata: %1").arg(err).toStdString());
  }
}

void Video::printInfo() const {
  QTextStream out(stdout);
  out << "File           : " << videoInfo.filePath << "\n";
  out << "MIME           : " << (videoInfo.mimeType.isEmpty() ? "Unknown" : videoInfo.mimeType) << "\n";
  out << "Size           : " << convertFileSizeToHumanReadable(videoInfo.fileBytes) << " (" << videoInfo.fileBytes
      << " bytes)\n";
  out << "Duration       : " << convertDurationMsToHumanReadable(videoInfo.durationMs) << " (" << videoInfo.durationMs
      << " ms)\n";
  if (videoInfo.resolution.isValid())
    out << "Resolution     : " << videoInfo.resolution.width() << "x" << videoInfo.resolution.height() << "\n";
  else
    out << "Resolution     : Unknown\n";

  out << "Video Codec    : " << (videoInfo.videoCodec.isEmpty() ? "Unknown" : videoInfo.videoCodec) << "\n";
  out << "Video Bitrate  : "
      << (videoInfo.videoBitRate > 0 ? QString::number(videoInfo.videoBitRate) + " bps" : "Unknown") << "\n";
  out << "Frame Rate     : "
      << (videoInfo.frameRate > 0.0 ? QString::number(videoInfo.frameRate, 'f', 3) + " fps" : "Unknown") << "\n";

  out << "Audio Codec    : " << (videoInfo.audioCodec.isEmpty() ? "Unknown" : videoInfo.audioCodec) << "\n";
  out << "Audio Bitrate  : "
      << (videoInfo.audioBitRate > 0 ? QString::number(videoInfo.audioBitRate) + " bps" : "Unknown") << "\n";
  out << "Orientation    : " << (videoInfo.orientation.isEmpty() ? "Unknown" : videoInfo.orientation) << "\n";
  out.flush();
}

void Video::checkVideoFilePath(const QString &path) {
  QFileInfo fi(path);
  if (!fi.exists()) {
    throw std::runtime_error(QString("File does not exist: %1").arg(path).toStdString());
  }
  if (!fi.isReadable()) {
    throw std::runtime_error(QString("File is not readable: %1").arg(path).toStdString());
  }
  if (fi.isDir()) {
    throw std::runtime_error(QString("Path is a directory, not a file: %1").arg(path).toStdString());
  }
}

void Video::validateMIMEtype(const QString &path) {
  QMimeDatabase   db;
  const QMimeType mt = db.mimeTypeForFile(path);
  if (!mt.isValid()) {
    throw std::runtime_error(QString("Cannot determine MIME type for file: %1").arg(path).toStdString());
  }
  if (!mt.name().startsWith("video/")) {
    // https://doc.qt.io/qt-6/qtcore-mimetypes-mimetypebrowser-example.html
    throw std::runtime_error(QString("Not a video file (MIME: %1).").arg(mt.name()).toStdString());
  }
}

QString Video::convertFileSizeToHumanReadable(qint64 bytes) {
  // https://stackoverflow.com/a/30958189
  double size = static_cast<double>(bytes);

  QStringList list;
  list << "KB" << "MB" << "GB" << "TB";
  QStringListIterator i(list);
  QString             unit("bytes");

  while (size >= 1000.0 && i.hasNext()) {
    unit = i.next();
    size /= 1000.0;
  }
  return QString().setNum(size, 'f', 2) + " " + unit;
}

QString Video::convertDurationMsToHumanReadable(qint64 ms) {
  if (ms <= 0)
    return "0s";
  qint64 totalSeconds = ms / 1000;
  qint64 hours        = totalSeconds / 3600;
  qint64 minutes      = (totalSeconds % 3600) / 60;
  qint64 seconds      = totalSeconds % 60;
  // https://stackoverflow.com/a/55761192
  return QString("%1:%2:%3")
      .arg(hours, 2, 10, QChar('0'))
      .arg(minutes, 2, 10, QChar('0'))
      .arg(seconds, 2, 10, QChar('0'));
}
