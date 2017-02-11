//#define DEBUG

// Config version
#define CONFIG_VERSION "SM1"

// SyrotaAutomation parameters
#define RS485_CONTROL_PIN 2
#define NET_ADDRESS "SewerMonitor"

struct configuration_t {
  char checkVersion[4]; // This is for detection if we have right settings or not
  unsigned long baudRate; // Serial/RS-485 rate: 9600, 14400, 19200, 28800, 38400, 57600, or 
  unsigned int laserReadInterval; // How often to read laser sensor (milliseconds)
};
