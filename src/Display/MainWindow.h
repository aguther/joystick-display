#pragma once

#include <QMainWindow>
#include <Windows.h>
#include <QAction>
#include <QtGui>
#include "InputData.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
 Q_OBJECT

 public:
  explicit MainWindow(
      QWidget *parent = nullptr,
      bool isTopMost = false,
      bool withFrame = false,
      int width = 250,
      int height = 270
  );

  ~MainWindow() override;

 public slots:
  void connectClicked();

  void updateData(
      InputData data,
      double updateTime);

 signals:
  void connect(
      int configurationIndex
  );

  void disconnect();

 protected:
  void contextMenuEvent(QContextMenuEvent *event) override;

  void mousePressEvent(QMouseEvent *evt) override {
    previousPosition = evt->globalPos();
  }

  void mouseMoveEvent(QMouseEvent *evt) override {
    const QPoint delta = evt->globalPos() - previousPosition;
    move(x() + delta.x(), y() + delta.y());
    previousPosition = evt->globalPos();
  }

 private:
  Ui::MainWindow *ui;
  QAction *connectAction;
  QAction *exitAction;

  QPoint previousPosition;
};
