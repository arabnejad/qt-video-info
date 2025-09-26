#pragma once
#include "IMediaMetadataProvider.h"
#include <QString>
#include <memory>

class Video {
public:
  explicit Video(QString path, std::unique_ptr<IMediaMetadataProvider> provider);

  void printInfo() const;

  VideoInfo &info() {
    return videoInfo;
  }

private:
  QString                                 filePath;
  VideoInfo                               videoInfo;
  std::unique_ptr<IMediaMetadataProvider> mediaMetadataprovider;

  static void    checkVideoFilePath(const QString &path);
  static void    validateMIMEtype(const QString &path);
  static QString convertFileSizeToHumanReadable(qint64 bytes);
  static QString convertDurationMsToHumanReadable(qint64 ms);
};
