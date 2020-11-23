#include "JoystickDisplay.h"

JoystickDisplay::JoystickDisplay(
    QWidget *parent
) : QChartView(parent) {
  init();
}

void JoystickDisplay::init() {
  // setup chart
  chartObject = new QChart();
  chartObject->legend()->hide();
  chartObject->setAnimationOptions(QChart::SeriesAnimations);
  chartObject->setAnimationDuration(50);
  chartObject->setTheme(QChart::ChartThemeDark);
  chartObject->setContentsMargins(QMargins(-63, -36, -43, -50));
  chartObject->legend()->setVisible(false);

  // setup x axis
  axisX->setRange(-1, 1);
  axisX->setLabelsVisible(false);
  axisX->setTitleVisible(false);
  axisX->setTickType(QValueAxis::TicksDynamic);
  axisX->setTickAnchor(0.0);
  axisX->setTickInterval(1.0);
  chartObject->addAxis(axisX, Qt::AlignBottom);

  // setup y axis
  axisY->setRange(-1, 1);
  axisY->setLabelsVisible(false);
  axisY->setTitleVisible(false);
  axisY->setTickType(QValueAxis::TicksDynamic);
  axisY->setTickAnchor(0.0);
  axisY->setTickInterval(1.0);
  chartObject->addAxis(axisY, Qt::AlignLeft);

  // add chart to view
  setChart(chartObject);
  setRenderHint(QPainter::Antialiasing);

  // define points
  //points->setName("");
  points->setMarkerShape(QScatterSeries::MarkerShapeCircle);
  points->setColor(QColor("red"));
  points->setBorderColor(QColor("red"));
  points->setMarkerSize(15.0);

  chartObject->addSeries(points);
  chartObject->legend()->setMarkerShape(QLegend::MarkerShapeFromSeries);

  // attach axes
  points->attachAxis(axisX);
  points->attachAxis(axisY);

  // create and start refresh timer (replaces buffers)
  updateTimer = new QTimer(this);
  QObject::connect(
      updateTimer,
      &QTimer::timeout,
      this,
      &JoystickDisplay::replaceBuffers,
      Qt::ConnectionType::QueuedConnection
  );
  updateTimer->start(UPDATE_RATE_MILLISECONDS);
}

void JoystickDisplay::update(
    double x,
    double y
) {
  buffer.clear();
  buffer.append(QPointF(x, y));
}

void JoystickDisplay::replaceBuffers() {
  points->replace(buffer);
}
