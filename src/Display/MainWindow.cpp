#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(
    QWidget *parent,
    bool isTopMost,
    bool withFrame,
    int width,
    int height
) : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  ui->displayJoystick->update(0, 0);
  ui->displayRudder->update(0, 0);

  QObject::connect(
      ui->actionConnect,
      &QAction::triggered,
      this,
      &MainWindow::connectClicked
  );

  connectAction = new QAction("Connect", this);
  connectAction->setCheckable(true);
  QObject::connect(connectAction, &QAction::triggered, this, &MainWindow::connectClicked);

  exitAction = new QAction("Exit", this);
  QObject::connect(exitAction, &QAction::triggered, this, &MainWindow::close);

  // set windows options
  Qt::WindowFlags flags = Qt::Window;
  if (!withFrame) {
    flags |= Qt::FramelessWindowHint;
  }
  if (isTopMost) {
    flags |= Qt::WindowStaysOnTopHint;
  }
  setWindowFlags(flags);

  // set size
  setGeometry(0, 0, width, height);
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) {
  QMenu menu(this);
  menu.addAction(connectAction);
  menu.addAction(exitAction);
  menu.exec(event->globalPos());
}

void MainWindow::connectClicked() {
  if (connectAction->isChecked()) {
    emit connect(0);
  } else {
    emit disconnect();
  }
}

void MainWindow::updateData(
    InputData data,
    double updateTime
) {
  ui->displayJoystick->update(-1.0 * data.delta_xi_pos, data.delta_eta_pos);
  ui->displayRudder->update(-1.0 * data.delta_zeta_pos, 0);
}
