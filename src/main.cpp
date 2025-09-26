#include "ArgParser.h"
#include "Video.h"
#include "VideoInfoExtractor.h"

#include <QCoreApplication>
#include <QTextStream>
#include <memory>
#include <stdexcept>

static void printError(const QString &msg) {
  QTextStream err(stderr);
  err << "Error: " << msg << "\n";
  err.flush();
}

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("qt_video_info");
  QCoreApplication::setApplicationVersion("1.0");

  ProgramOptions opts;
  auto [ok, reason] = ArgParser::parse(app, opts);
  if (!ok) {
    printError(reason);
    return 1;
  }

  try {
    auto  provider = std::make_unique<VideoInfoExtractor>();
    Video v(opts.inputPath, std::move(provider));
    v.printInfo();
  } catch (const std::exception &e) {
    printError(QString::fromStdString(e.what()));
    return 2;
  }

  return 0;
}
