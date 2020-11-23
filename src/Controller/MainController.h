#pragma once

#include <Windows.h>
#include <SimConnectData.h>
#include <SimConnectDataDefinition.h>
#include <SimConnectDataInterface.h>
#include <SimConnectInputInterface.h>

#include <QTimer>
#include <QDateTime>
#include <QThread>

#include "InputData.h"

class MainController : public QObject {
 Q_OBJECT

 public:
  MainController();

  ~MainController() override;

 signals:
  void simConnectConnected();

  void simConnectDisconnected();

  void simConnectFailed();

  void dataUpdated(
      InputData data,
      double updateTime
  );

 public slots:
  void start(
      int configurationIndex
  );

  void stop();

  void update();

 private:
  const int STEP_TIME = 50;

  QTimer *updateTimer = nullptr;
  QDateTime lastUpdateTime;

  std::shared_ptr<simconnect::toolbox::connection::SimConnectDataDefinition> dataDefinitionInput;
  std::shared_ptr<simconnect::toolbox::connection::SimConnectData> simConnectDataInput;
  simconnect::toolbox::connection::SimConnectInputInterface simConnectInterfaceInput;

  bool isConnected = false;

  void simConnectSetupInputData();
};
