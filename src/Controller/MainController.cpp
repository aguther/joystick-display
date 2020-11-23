#include "MainController.h"

#include <QDebug>
#include <cmath>
#include <InputData.h>

using namespace std;
using namespace simconnect::toolbox::connection;

MainController::MainController() {
  updateTimer = new QTimer(this);
  updateTimer->setTimerType(Qt::PreciseTimer);
  QObject::connect(
      updateTimer,
      &QTimer::timeout,
      this,
      &MainController::update
  );
}

MainController::~MainController() {
  if (updateTimer != nullptr) {
    updateTimer->stop();
    delete updateTimer;
  }
}

void MainController::start(
    int configurationIndex
) {
  // prepare data definitions
  simConnectSetupInputData();

  // connect to FS2020
  bool result = simConnectInterfaceInput.connect(
      configurationIndex,
      "joystick-input",
      *dataDefinitionInput,
      simConnectDataInput,
      SIMCONNECT_GROUP_PRIORITY_HIGHEST
  );

  // check if everything worked
  if (!result) {
    stop();
    return;
  }

  // we are connected
  isConnected = true;

  // start update loop
  updateTimer->start(STEP_TIME);
}

void MainController::stop() {
  // stop update loop
  updateTimer->stop();

  // disconnect from FS2020
  if (isConnected) {
    // close connection
    simConnectInterfaceInput.disconnect();

    // reset data objects
    simConnectDataInput.reset();
    dataDefinitionInput.reset();

    // set flag
    isConnected = false;
  }
}

void MainController::update() {
  // get update time
  auto now = QDateTime::currentDateTimeUtc();
  auto updateTime = lastUpdateTime.msecsTo(now) / 1000.0;
  lastUpdateTime = now;

  // ******************************************************************************************************************

  // read inputs
  simConnectInterfaceInput.readData();
  InputData inputData = {
      any_cast<double>(simConnectDataInput->get(0)),
      any_cast<double>(simConnectDataInput->get(1)),
      any_cast<double>(simConnectDataInput->get(2))
  };

  // ******************************************************************************************************************

  // fire signal that data was updated
  emit dataUpdated(
      inputData,
      updateTime
  );
}

void MainController::simConnectSetupInputData() {
  // create object
  dataDefinitionInput = make_shared<SimConnectDataDefinition>();

  // add data definitions
  dataDefinitionInput->add(SimConnectVariable("AXIS_ELEVATOR_SET", "FALSE"));
  dataDefinitionInput->add(SimConnectVariable("AXIS_AILERONS_SET", "FALSE"));
  dataDefinitionInput->add(SimConnectVariable("AXIS_RUDDER_SET", "FALSE"));

  // create data object holding the actual data
  simConnectDataInput = make_shared<SimConnectData>(*dataDefinitionInput);
}
