#include <EEPROMex.h>
#include <Wire.h>
#include <VL53L0X.h>
#include <SyrotaAutomation1.h>
#include "config.h"
#include "LaserSensing.h"

SyrotaAutomation net = SyrotaAutomation(RS485_CONTROL_PIN);

VL53L0X sensor;


// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

//#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
//#define HIGH_ACCURACY

struct configuration_t conf = {
  CONFIG_VERSION,
  // Default values for config
  9600UL, //unsigned long baudRate; // Serial/RS-485 rate: 9600, 14400, 19200, 28800, 38400, 57600, or 
  1000U, //unsigned int laserReadInterval; // How often to read laser sensor (milliseconds)
};

LaserSensing laser(3000,10);

unsigned long lastLaserReadTime = 0;

void setup()
{
  readConfig();
  // Set device ID
  strcpy(net.deviceID, NET_ADDRESS);
  Serial.begin(9600);
  
  Wire.begin();

  sensor.init();
  sensor.setTimeout(500);

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor.setMeasurementTimingBudget(200000);
#endif
}

void readConfig()
{
  // Check to make sure config values are real, by looking at first 3 bytes
  if (EEPROM.read(0) == CONFIG_VERSION[0] &&
    EEPROM.read(1) == CONFIG_VERSION[1] &&
    EEPROM.read(2) == CONFIG_VERSION[2]) {
    EEPROM.readBlock(0, conf);
  } else {
    // Configuration is invalid, so let's write default to memory
    saveConfig();
  }
}

void saveConfig()
{
  EEPROM.writeBlock(0, conf);
}

void loop()
{
  if (net.messageReceived()) {
    char buf[100];
    if (net.assertCommand("getDistance")) {
      net.sendResponse(itoa(laser.getValue(), buf, 10));
    } else if (net.assertCommand("getPercentValid")) {
      net.sendResponse(itoa(laser.percentValid(), buf, 10));
    } else if (net.assertCommandStarts("set", buf)) {
      processSetCommands();
    } else {
      net.sendResponse("Unrecognized command");
    }
  }
  if (millis() - lastLaserReadTime > conf.laserReadInterval) {
    readDistance();
  }
}

void readDistance() {
  int res = sensor.readRangeSingleMillimeters();
  laser.reading(res);
  lastLaserReadTime = millis();
}

// Write to the configuration when we receive new parameters
void processSetCommands()
{
  char buf[100];
  if (net.assertCommandStarts("setBaudRate:", buf)) {
    long tmp = strtol(buf, NULL, 10);
    // Supported: 9600, 14400, 19200, 28800, 38400, 57600, or 115200
    if (tmp == 9600 ||
      tmp == 14400 ||
      tmp == 19200 ||
      tmp == 28800 ||
      tmp == 38400 ||
      tmp == 57600 ||
      tmp == 115200
    ) {
      conf.baudRate = tmp;
      saveConfig();
      net.sendResponse("OK");
      Serial.end();
      Serial.begin(tmp);
    } else {
      net.sendResponse("ERROR");
    }
  } else if (net.assertCommandStarts("setLaserReadInterval:", buf)) {
    unsigned int tmp = strtol(buf, NULL, 10);
    if (tmp > 0 && tmp < 60000UL) {
      conf.laserReadInterval = tmp;
      saveConfig();
      net.sendResponse("OK");
    } else {
      net.sendResponse("ERROR");
    }
  } else {
    net.sendResponse("Unrecognized");
  }
}
