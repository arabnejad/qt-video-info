#pragma once
#include <QString>
#include <utility>

class QCoreApplication;

struct ProgramOptions {
  QString inputPath;
};

class ArgParser {
public:
  static std::pair<bool, QString> parse(QCoreApplication &app, ProgramOptions &opts);
};
