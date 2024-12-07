#include <ArduinoOTA.h>
#include <Ticker.h>
#include <AsyncMqttClient.h> 
#include <time.h>

#include "hh_defines.h"
#include "hh_utilities.h"
#include "hh_cntrl.h"

// Folling line added to stop compilation error suddenly occuring in 2024???
#include "ESPAsyncDNSServer.h"

#include <config.h>

#define ESP8266_DRD_USE_RTC true
#define ESP_DRD_USE_LITTLEFS false
#define ESP_DRD_USE_SPIFFS false
#define ESP_DRD_USE_EEPROM false
#define DOUBLERESETDETECTOR_DEBUG true
#include <ESP_DoubleResetDetector.h>

//***********************
// Template functions
//***********************
bool onMqttMessageAppExt(char *, char *, const AsyncMqttClientMessageProperties &, const size_t &, const size_t &, const size_t &);
bool onMqttMessageAppCntrlExt(char *, char *, const AsyncMqttClientMessageProperties &, const size_t &, const size_t &, const size_t &);
void appMQTTTopicSubscribe();
void telnet_extension_1(char);
void telnet_extension_2(char);
void telnet_extensionHelp(char);
void startTimesReceivedChecker();
void processCntrlTOD_Ext();

//******************************
// defined in asyncConnect.cpp
//******************************
extern void mqttTopicsubscribe(const char *topic, int qos);
extern void platform_setup(bool);
extern void handleTelnet();
extern void printTelnet(String);
extern AsyncMqttClient mqttClient;
extern void wifiSetupConfig(bool);
extern templateServices coreServices;
extern char ntptod[MAX_CFGSTR_LENGTH];

//*************************************
// defined in cntrl.cpp
//*************************************
extern cntrlState *cntrlObjRef; // pointer to cntrlState
cntrlState controllerState;		// Create and set defaults


//#define WDCntlTimes  	"/house/cntrl/portable-switch/wd-control-times" 		// Times received from either UI or Python app
//#define WECntlTimes  	"/house/cntrl/portable-switch/we-control-times" 		// Times received from either UI or MySQL via Python app
//#define runtimeState    "/house/cntrl/portable-switch/runtime-state" 	 		// published state: ON, OFF, and AUTO
//#define LightState      "/house/cntrl/portable-switch/light-state"              // ON or OFF
//#define WDUICmdState 	"/house/cntrl/portable-switch/wd-command"		 		// UI Button press received
//#define WEUICmdState 	"/house/cntrl/portable-switch/we-command"		 		// UI Button press received
//#define RefreshID		"PS01"												 	// the key send to Python app to refresh Cntroler state	

#define DRD_TIMEOUT 3
#define DRD_ADDRESS 0

DoubleResetDetector *drd;

// defined in telnet.cpp
extern int reporting;
extern bool telnetReporting;

//
// Application specific 
//

//String deviceName   = "portable-switch";
//String deviceType   = "CNTRL";
//String app_id		= "PSC";		// configure

int relay_pin 		= D1;			// wemos D1. LIght on or off (Garden lights)
int relay_pin_pir   = D2;	        // wemos D2. LIght on or off (Garage Path)
int ManualStatus    = D3;           // Manual over ride.  If low then lights held on manually
int LIGHTSON        = 1;
int LIGHTSOFF       = 0;
int LIGHTSAUTO      = 3;			// Not using this at the moment

bool bManMode       = false; 		// true = Manual, false = automatic

//const char *oh3CommandTrigger   = "/house/cntrl/outside-lights-front/pir-command";	    // Event fron the PIR detector (front porch: PIRON or PIROFF
//const char *oh3StateManual		= "/house/cntrl/portable-switch/manual-state";	 // 	Status of the Manual control switch control MAN or AUTO

//************************
// Application specific
//************************
bool processCntrlMessageApp_Ext(char *, const char *, const char *, const char *);
void processAppTOD_Ext();

devConfig espDevice;

Ticker configurationTimesReceived;
bool timesReceived;

void setup()
{
    //***************************************************
    // Set-up Platform - hopefully dont change this
    //***************************************************
    bool configWiFi = false;
    Serial.begin(115200);
    while (!Serial)
        delay(300);

    espDevice.setup(deviceName, deviceType);       
    Serial.println(startUpMessage);
    Serial.println(ARDUINO_BOARD);

    drd = new DoubleResetDetector(DRD_TIMEOUT, DRD_ADDRESS);
    if (drd->detectDoubleReset())
    {
        configWiFi = true;
    }

	// this app is a contoller
	// configure the MQTT topics for the Controller
	controllerState.setCntrlName((String) app_id + "01");
	controllerState.setRefreshID(RefreshID);
	controllerState.setCntrlObjRef(controllerState);

    // Platform setup: Set up and manage: WiFi, MQTT and Telnet
    platform_setup(configWiFi);

    //***********************
    // Application setup
    //***********************
    pinMode(relay_pin, OUTPUT);
	pinMode(relay_pin_pir, OUTPUT);
	pinMode(ManualStatus, INPUT);
	digitalWrite(relay_pin, LIGHTSOFF);
	digitalWrite(relay_pin_pir, LIGHTSOFF);

    configurationTimesReceived.attach(30, startTimesReceivedChecker); 	
}

void loop()
{
    int pinVal = 0;
    char logString[MAX_LOGSTRING_LENGTH];
    drd->loop();

    // Go look for OTA request
    ArduinoOTA.handle();

    handleTelnet();

    pinVal = digitalRead(ManualStatus);
	if (pinVal == 0) // means manual switch ON and lights forced to stay on
	{
		bManMode = true;
                memset(logString, 0, sizeof logString);
		        sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "PSC Manually Held ON");
				mqttLog(logString, REPORT_WARN, true, true);

				app_WD_on(&controllerState);  // FIXTHIS WD or WE
                mqttClient.publish(oh3StateManual, 0, true, "MAN");
	}
	else
	{
		bManMode = false;
		// mqttClient.publish(oh3StateManual, 1, true, "AUTO"); //FIXTHIS  I thing we cant assume auto - need to get state prior
	}
}

//****************************************************************
// Process any application specific inbound MQTT messages
// Return False if none
// Return true if an MQTT message was handled here
//****************************************************************
bool onMqttMessageAppExt(char *topic, char *payload, const AsyncMqttClientMessageProperties &properties, const size_t &len, const size_t &index, const size_t &total)
{
    (void)payload;

    char mqtt_payload[len+1];
    mqtt_payload[len] = '\0';
    strncpy(mqtt_payload, payload, len);

	mqttLog(mqtt_payload, REPORT_DEBUG, true, true);

	if (strcmp(topic, oh3CommandTrigger) == 0)
	{
		if (strcmp(mqtt_payload, "PIRON") == 0)
		{
			mqttLog("PIRON Detected", REPORT_INFO, true, true);
			digitalWrite(relay_pin_pir, LIGHTSON);
			digitalWrite(relay_pin, LIGHTSON);
			//controllerState.onMqttMessageCntrlExt(topic, payload, properties, len, index, total); // FIXTHIS what is this for
            return true;
		}

		if (strcmp(mqtt_payload, "PIROFF") == 0)
		{
			mqttLog("PIROFF Detected", REPORT_INFO, true, true);
			if (bManMode != true)
				digitalWrite(relay_pin_pir, LIGHTSOFF);
				digitalWrite(relay_pin, LIGHTSOFF);

			//controllerState.onMqttMessageCntrlExt(topic, payload, properties, len, index, total); // FIXTHIS what is this for
            return true;
		}
    }   
	//controllerState.onMqttMessageCntrlExt(topic, payload, properties, len, index, total); 
    return false;
}

void processAppTOD_Ext()
{
	mqttLog("PSC01 Application Processing TOD", REPORT_INFO, true, true);
}

bool processCntrlMessageApp_Ext(char *mqttMessage, const char *onMessage, const char *offMessage, const char *commandTopic)
{
	if (strcmp(mqttMessage, "SET") == 0)
	{
		mqttClient.publish(oh3StateManual, 0, true, "AUTO"); // This just sets the UI to show that MAN start is OFF
		return true;
	}
	return false;
}
//***************************************************
// Connected to MQTT Broker 
// Subscribe to application specific topics
//***************************************************
void appMQTTTopicSubscribe()
{	

    mqttTopicsubscribe(oh3CommandTrigger, 2);

	controllerState.setWDCntrlTimesTopic(WDCntlTimes);
	controllerState.setWDUIcommandStateTopic(WDUICmdState);
	controllerState.setWDCntrlRunTimesStateTopic(runtimeState);

	controllerState.setWECntrlTimesTopic(WECntlTimes);
	controllerState.setWEUIcommandStateTopic(WEUICmdState);
	controllerState.setWECntrlRunTimesStateTopic(runtimeState);
}

//**********************************************
// Controller Notification : Weekday ON
//**********************************************
void app_WD_on(void *cid)
{
    cntrlState *obj = (cntrlState *)cid;
	String msg = obj->getCntrlName() + + " " +  "WD ON";
	
	if (coreServices.getWeekDayState() == 1)			// 1 means weekday
	{
		digitalWrite(relay_pin, LIGHTSON);
		controllerState.setOutputState(1);  // Feedback to the Controller the output of the requested action

		mqttLog(msg.c_str(), REPORT_INFO, true, true);
		mqttClient.publish(LightState, 0, false, "ON");		// QoS = 0
	}	
}

//**********************************************
// Controller Notification : Weekday OFF
//**********************************************
void app_WD_off(void  *cid)
{
	cntrlState *obj = (cntrlState *)cid;
	String msg = obj->getCntrlName() + + " " +  "WD OFF";

	if (coreServices.getWeekDayState() == 1)			// 1 means weekday
	{
		digitalWrite(relay_pin, LIGHTSOFF);
		controllerState.setOutputState(0); // Feedback to the Controller the output of the requested action
        
		mqttLog(msg.c_str(), REPORT_INFO, true, true);
		mqttClient.publish(LightState, 0, false, "OFF");		// QoS = 0
	}	
}

//**********************************************
// Controller Notification : Weekend ON
//**********************************************
void app_WE_on(void *cid)
{
	cntrlState *obj = (cntrlState *)cid;
	String msg = obj->getCntrlName() + + " " +  "WE ON";

	if (coreServices.getWeekDayState() == 0)			// 0 means weekend
	{
		digitalWrite(relay_pin, LIGHTSON);
		controllerState.setOutputState(1); // 1=ON, Feedback to the Controller the output of the requested action

		mqttLog(msg.c_str(), REPORT_INFO, true, true);
		mqttClient.publish(LightState, 0, false, "ON");		// QoS = 0
	}
}

//**********************************************
// Controller Notification : Weekend OFF
//**********************************************
void app_WE_off(void *cid)
{
	cntrlState *obj = (cntrlState *)cid;
	String msg = obj->getCntrlName() + + " " +  "WE OFF";

	if (coreServices.getWeekDayState() == 0)			// 0 means weekend
	{
		digitalWrite(relay_pin, LIGHTSOFF);	
		controllerState.setOutputState(0); // 0=OFF, Feedback to the Controller the output of the requested action

		mqttLog(msg.c_str(), REPORT_INFO, true, true);
		mqttClient.publish(LightState, 0, false, "OFF");		// QoS = 0
	}
}

//**********************************************
// Controller Notification : Weekday AUTO
//**********************************************
void app_WD_auto(void *cid)
{
	cntrlState *obj = (cntrlState *)cid;
	String msg = obj->getCntrlName() + " WD AUTO";

	if (coreServices.getWeekDayState() == 1)			// 1 means weekday
	{
		mqttLog(msg.c_str(), REPORT_INFO, true, true);
							
		//mqttClient.publish(getWDCntrlRunTimesStateTopic().c_str(), 0, true, "AUTO");
		mqttClient.publish(obj->getWDUIcommandStateTopic().c_str(), 0, false, "SET"); //
	}
}
//**********************************************
// Controller Notification : Weekend AUTO
//**********************************************
void app_WE_auto(void  *cid)
{
	cntrlState *obj = (cntrlState *)cid;
	String msg = obj->getCntrlName() + " WE AUTO";

	if (coreServices.getWeekDayState() == 0)			// 0 means weekend
	{
		mqttLog(msg.c_str(), REPORT_INFO, true, true);
	
		//mqttClient.publish(getWECntrlRunTimesStateTopic().c_str(), 0, true, "AUTO");
		mqttClient.publish(obj->getWEUIcommandStateTopic().c_str(), 0, false, "SET");
	}
}

void startTimesReceivedChecker()
{
    controllerState.runTimeReceivedCheck(); 
}

void processCntrlTOD_Ext()
{
	controllerState.processCntrlTOD_Ext();
}
void telnet_extension_1(char c)
{
	controllerState.telnet_extension_1(c);
}
// Process any application specific telnet commannds
void telnet_extension_2(char c)
{
	char logString[51];
	memset(logString, 0, sizeof logString);
	sprintf(logString,
				"\r%s%i\n",
				"Switch State\t", digitalRead(relay_pin));

	printTelnet((String)logString);
}

// Process any application specific telnet commannds
void telnet_extensionHelp(char c)
{
	printTelnet((String) "x\t\tSome description");
}

bool onMqttMessageAppCntrlExt(char *topic, char *payload, const AsyncMqttClientMessageProperties &properties, const size_t &len, const size_t &index, const size_t &total)
{
	return controllerState.onMqttMessageCntrlExt(topic, payload, properties, len, index, total);
}
