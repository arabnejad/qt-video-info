#pragma once
#include "IMediaMetadataProvider.h"

class VideoInfoExtractor : public IMediaMetadataProvider {
public:
  VideoInfoExtractor() = default;

  // Extract metadata from 'filePath'
  // meta data for the file will be saved into 'videoInfo'
  // On failure returns false and sets 'errorMessage'
  bool extract(const QString &filePath, VideoInfo &videoInfo, QString &errorMessage) override;

private:
  int timeoutMs = 10000; // 10 seconds default
};
