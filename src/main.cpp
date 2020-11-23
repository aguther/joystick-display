#include "Display/MainWindow.h"
#include "Controller/MainController.h"

#include <QApplication>

int main(
    int argc,
    char *argv[]
) {
  // create application
  QApplication application(argc, argv);

  QCommandLineParser parser;
  parser.setApplicationDescription("Test helper");
  parser.addHelpOption();

  QCommandLineOption noTopOption(
      QStringList() << "no-top",
      "Start application with *not* as top-most window."
  );
  parser.addOption(noTopOption);

  QCommandLineOption frameOption(
      QStringList() << "frame",
      "Start application with the normal window frame."
  );
  parser.addOption(frameOption);

  QCommandLineOption heightOption(
      QStringList() << "height",
      "Set the application window height.",
      "height",
      "270"
  );
  parser.addOption(heightOption);

  QCommandLineOption widthOption(
      QStringList() << "width",
      "Set the application window width.",
      "width",
      "250"
  );
  parser.addOption(widthOption);

  parser.process(application);
  const QStringList args = parser.positionalArguments();

  // create main window
  MainWindow mainWindow(
      nullptr,
      !parser.isSet(noTopOption),
      parser.isSet(frameOption),
      parser.value(widthOption).toInt(),
      parser.value(heightOption).toInt()
  );
  // show main window
  mainWindow.show();

  // create main controller
  MainController mainController;

  // connect signals and slots
  QObject::connect(
      &mainController,
      &MainController::dataUpdated,
      &mainWindow,
      &MainWindow::updateData
  );

  QObject::connect(
      &mainWindow,
      &MainWindow::connect,
      &mainController,
      &MainController::start
  );

  QObject::connect(
      &mainWindow,
      &MainWindow::disconnect,
      &mainController,
      &MainController::stop
  );

  // execute
  int result = QApplication::exec();

  // ensure everything is stopped
  mainController.stop();

  // return
  return result;
}
