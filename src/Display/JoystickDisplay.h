#pragma once

#include <QtCharts>

class JoystickDisplay : public QChartView {
 public:
  explicit JoystickDisplay(QWidget *parent);

  void update(double x, double y);

  const int UPDATE_RATE_MILLISECONDS = 50;

 public slots:
  void replaceBuffers();

 private:
  void init();

  QTimer *updateTimer = nullptr;

  QChart *chartObject = nullptr;

  QValueAxis *axisX = new QValueAxis();
  QValueAxis *axisY = new QValueAxis();

  QVector<QPointF> buffer;
  QScatterSeries *points = new QScatterSeries();
};
