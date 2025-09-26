#include "ArgParser.h"
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFileInfo>

std::pair<bool, QString> ArgParser::parse(QCoreApplication &app, ProgramOptions &opts) {

  QCommandLineParser parser;
  parser.setApplicationDescription("Console tool to print video metadata.");
  parser.addHelpOption();
  parser.addVersionOption();

  parser.addPositionalArgument("video-file", "Path to a video file.");

  parser.process(app);

  const QStringList args = parser.positionalArguments();
  if (args.size() != 1) {
    return {false, parser.helpText()};
  }

  opts.inputPath = args.first();

  return {true, QString()};
}
