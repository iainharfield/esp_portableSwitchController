/****************************************************************************************************************************
  defines.h
  AsyncMqttClient_Generic is a library for ESP32, ESP8266, Protenta_H7, STM32F7, etc. with current AsyncTCP support

  Based on and modified from :

  1) async-mqtt-client (https://github.com/marvinroger/async-mqtt-client)

  Built by Khoi Hoang https://github.com/khoih-prog/AsyncMqttClient_Generic
 ***************************************************************************************************************************************/

#ifndef defines_h
#define defines_h

#include <Arduino.h>
#include <TZ.h>

#define _ASYNC_MQTT_LOGLEVEL_ 1

#define MAX_CFGSTR_LENGTH 51
#define MAX_LOGSTRING_LENGTH 501

#define TELNET_PORT 23

#define MY_TZ TZ_Europe_London
#define MY_NTP_SERVER "at.pool.ntp.org"

#define p_mqttBrokerIP_Label "mqttBrokerIP"
#define p_mqttBrokerPort_Label "mqttBrokerPort"

// MQTT Topic Names
#define oh3CommandIOT "/house/service/iot-command"             // e.g. IOT-IDENTITY, IOT-RESET, IOT-RESTART, IOT-SWITCH-CONFIG
#define oh3StateLog "/house/service/log"                       // Log messages
#define oh3CommandTOD "/house/service/time-of-day"             // Time of day broadcast from OpenHab
#define oh3StateIOTRefresh "/house/service/iot-device-refresh" // Request Refresh of Contol times ( not needed by all apps)

#define REPORT_INFO 0
#define REPORT_WARN 1
#define REPORT_DEBUG 2

//***************************************************************
// Controller Specific MQTT Topics and config
//***************************************************************
#define AUTOMODE 0 // Normal running mode - Heating times are based on the 3 time zones
#define NEXTMODE 1 // Advances the control time to the next zone. FIX-THIS: Crap description
#define ONMODE 2   // Permanently ON.  Heat is permanently requested. Zones times are ignored
#define OFFMODE 3  // Permanently OFF.  Heat is never requested. Zones times are ignored

#define SBUNKOWN 0
#define SBON 1
#define SBOFF 2

#define ZONE1 0
#define ZONE2 1
#define ZONE3 2

#define ZONEGAP 9

class devConfig
{
  String devName;
  String devType;
  bool configWifiOnboot;

public:
  devConfig() {}
  devConfig(String name, String type)
  {
    devName = name;
    devType = type;
    configWifiOnboot = false;
  }
  void setup(String name, String type)
  {
    devName = name;
    devType = type;
  }
  String getName()
  {
    return devName;
  }
  String getType()
  {
    return devType;
  }
  void setwifiConfigOnboot(bool bValue)
  {
    configWifiOnboot = bValue;
  }
  bool getwifiConfigOnboot(bool bValue)
  {
    return configWifiOnboot;
  }
};

class templateServices
{
  bool weekDay = false; // initialise FIXTHIS
  int dayNumber;
  int timeNow = 0;
  bool ohTODReceived = false;

public:
  templateServices() {}
  templateServices(int dn)
  {
    dayNumber = dn;
    if (dn >= 1 && dn <= 5)
      weekDay = true;
    else
      weekDay = false;
  }
  void setup(int dn)
  {
    dayNumber = dn;
  }
  void setDayNumber(int dn)
  {
    dayNumber = dn;
    if (dn >= 1 && dn <= 5)
      weekDay = true;
    else
      weekDay = false;
  }
  void setTimeNow( int tn)
  {
    timeNow = tn;
  }
  void setOHTODReceived( bool ohtod)
  {
    ohTODReceived = ohtod;
  }
  bool getWeekDayState()
  {
    return weekDay;
  }
  int getTimeNow()
  {
    return timeNow;
  }
  bool getOHTODReceived()
  {
    return ohTODReceived;
  }
};

#endif // defines_h