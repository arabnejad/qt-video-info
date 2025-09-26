#include "VideoInfoExtractor.h"

#include <QEventLoop>
#include <QFileInfo>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include <QMimeDatabase>
#include <QMimeType>
#include <QTimer>
#include <QUrl>

bool VideoInfoExtractor::extract(const QString &filePath, VideoInfo &videoInfo, QString &errorMessage) {
  QFileInfo fi(filePath);

  videoInfo           = {};
  videoInfo.filePath  = filePath;
  videoInfo.fileBytes = fi.size();

  // Determine MIME
  QMimeDatabase   db;
  const QMimeType mt = db.mimeTypeForFile(fi);
  videoInfo.mimeType = mt.isValid() ? mt.name() : QString();

  QMediaPlayer player;
  player.setSource(QUrl::fromLocalFile(filePath));

  QEventLoop loop;
  bool       loaded = false;
  bool       failed = false;
  QString    failReason;

  QTimer timeout;
  timeout.setSingleShot(true);
  timeout.setInterval(timeoutMs);
  // https://github.com/Rain92/UltimateMangaReader/blob/master/src/utils.h#L33
  QObject::connect(&timeout, &QTimer::timeout, [&]() {
    failed     = true;
    failReason = QString("Timed out while loading media (%1 ms).").arg(timeoutMs);
    loop.quit();
  });
  timeout.start();

  // https://github.com/NikolovskaSimonaa/VideoPlayer/blob/main/recentfiles.cpp#L76
  QObject::connect(&player, &QMediaPlayer::mediaStatusChanged, [&](QMediaPlayer::MediaStatus status) {
    switch (status) {
    case QMediaPlayer::LoadedMedia:
    case QMediaPlayer::BufferedMedia:
      loaded = true;
      loop.quit();
      break;
    case QMediaPlayer::InvalidMedia:
      failed     = true;
      failReason = "Invalid media or unsupported format.";
      loop.quit();
      break;
    default:
      break;
    }
  });

  // https://github.com/thinhth510/DATN_QT/blob/main/model/mediafile.cpp#L129
  // Wait for metadata to be loaded
  loop.exec();

  if (!loaded || failed) {
    errorMessage = failReason.isEmpty() ? QString("Failed to load media metadata.") : failReason;
    return false;
  }

  const QMediaMetaData metaData = player.metaData();

  // https://doc.qt.io/qt-6/qmediametadata.html#Key-enum
  videoInfo.durationMs      = metaData.value(QMediaMetaData::Duration).toLongLong();
  videoInfo.frameRate       = metaData.value(QMediaMetaData::VideoFrameRate).toDouble();
  videoInfo.videoBitRate    = metaData.value(QMediaMetaData::VideoBitRate).toInt();
  videoInfo.audioBitRate    = metaData.value(QMediaMetaData::AudioBitRate).toInt();
  videoInfo.videoCodec      = metaData.value(QMediaMetaData::VideoCodec).toString();
  videoInfo.audioCodec      = metaData.value(QMediaMetaData::AudioCodec).toString();
  videoInfo.orientation     = metaData.value(QMediaMetaData::Orientation).toString();
  const QVariant dimensions = metaData.value(QMediaMetaData::Resolution);
  if (dimensions.isValid() && dimensions.canConvert<QSize>()) {
    videoInfo.resolution = dimensions.toSize();
  }

  return true;
}
