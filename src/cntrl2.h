


#ifndef cntrl2_h
#define cntrl2_h

#include "defines.h"
#include <Arduino.h>
#include <AsyncMqttClient_Generic.hpp>
//
// Defined in utilities
//
extern bool mqttLog(const char *, bool, bool);
// defined in telnet.cpp
extern int reporting;
extern bool telnetReporting;
//*************************************
// defined in asyncConnect.cpp
//*************************************
extern AsyncMqttClient mqttClient;
extern templateServices coreServices;
extern void mqttTopicsubscribe(const char *topic, int qos);
extern char ntptod[MAX_CFGSTR_LENGTH];

//
// defined in telnet.cpp
//
extern void printTelnet(String);

// defined in ??
extern int ohTimenow;
extern bool ohTODReceived;
extern devConfig espDevice;

//
// Application User exits
//
extern bool processCntrlMessageApp_Ext(char *, const char *, const char *, const char *);
extern void app_WD_on(void *);
extern void app_WD_off(void *);
extern void app_WE_on(void *);
extern void app_WE_off(void *);
extern void app_WD_auto(void *);
extern void app_WE_auto(void *);

void processCrtlTimes(char *, char (&ptr)[6][10], int lptr[6]);

class cntrlState
{ 
	int WDrunMode;
	int WDswitchBack;
	int WDzone;
	int WDHoldState;
	bool WDcntrlTimesReceived = false;
	bool WDcommandReceived = false;

	int WErunMode;
	int WEswitchBack;
	int WEzone;
	int WEHoldState;
	bool WEcntrlTimesReceived = false;
	bool WEcommandReceived = false;

	int WDlcntrlTimes[6];
	char WDcntrlTimes[6][10]{"0000", "0100", "0200", "0300", "0400", "0600"}; //  how big is each array element? 6 elements each element 10 characters long (9 + 1 for /0)
	int WElcntrlTimes[6];
	char WEcntrlTimes[6][10]{"0000", "0100", "0200", "0300", "0400", "0600"}; //  how big is each array element? 6 elements each element 10 characters long (9 + 1 for /0)

	String cntrlName;
	String refreshID;
	String WDCntrlRuntimeStateTopic;
	String WDcntrlTimesTopic;
	String WECntrlRuntimeStateTopic;
	String WEcntrlTimesTopic;
	String WDUICmdTopic;
	String WEUICmdTopic;
	
	cntrlState *cntrlObjRef;

	// Ticker configurationTimesReceived;
	bool timesReceived;

	// bool timeReceivedChecker();

public:
	cntrlState()
	{
		WDrunMode = AUTOMODE;
		WDswitchBack = SBUNKOWN;
		WDzone = ZONEGAP;
		WDHoldState = 9;
		WDcntrlTimesReceived = false;
		WDcommandReceived = false;

		WErunMode = AUTOMODE;
		WEswitchBack = SBUNKOWN;
		WEzone = ZONEGAP;
		WEHoldState = 9;
		WEcntrlTimesReceived = false;
		WEcommandReceived = false;

		refreshID = "UNDEF";
		cntrlName = "UNDEF";
	}
	cntrlState(int rm, int sb, int zn)
	{
		WDrunMode = rm;
		WDswitchBack = sb;
		WDzone = zn;
		WDHoldState = 9;
		WDcntrlTimesReceived = false;
		WDcommandReceived = false;

		WErunMode = rm;
		WEswitchBack = sb;
		WEzone = zn;
		WEHoldState = 9;
		WEcntrlTimesReceived = false;
		WEcommandReceived = false;

		refreshID = "UNDEF";
		cntrlName = "UNDEF";
	}
	void setup(int rm, int sb, int zn)
	{
		WDrunMode = rm;
		WDswitchBack = sb;
		WDzone = zn;
		WDHoldState = 9;
		WDcntrlTimesReceived = false;
		WDcommandReceived = false;

		WErunMode = rm;
		WEswitchBack = sb;
		WEzone = zn;
		WEHoldState = 9;
		WEcntrlTimesReceived = false;
		WEcommandReceived = false;

		refreshID = "UNDEF";
		cntrlName = "UNDEF";
	}
	String getCntrlName()
	{
		return cntrlName;
	}
	int getWDRunMode()
	{
		return WDrunMode;
	}
	int getWDSwitchBack()
	{
		return WDswitchBack;
	}
	int getWDZone()
	{
		return WDzone;
	}
	void setWDRunMode(int rm)
	{
		WDrunMode = rm;
	}
	void setWDSwitchBack(int sb)
	{
		WDswitchBack = sb;
	}
	void setWDHoldState(int hs)
	{
		WDHoldState = hs;
	}
	void setWEHoldState(int hs)
	{
		WEHoldState = hs;
	}
	void setWDZone(int zn)
	{
		WDzone = zn;
	}

	int getWERunMode()
	{
		return WErunMode;
	}
	int getWESwitchBack()
	{
		return WEswitchBack;
	}
	int getWEZone()
	{
		return WEzone;
	}
	void setWERunMode(int rm)
	{
		WErunMode = rm;
	}
	void setWESwitchBack(int sb)
	{
		WEswitchBack = sb;
	}
	void setWEZone(int zn)
	{
		WEzone = zn;
	}
	
	void setCntrlObjRef(cntrlState &obj)
	{
		cntrlObjRef = &obj;
	}


	void setWDCntrlTimesTopic(String timesTopic)
	{
		WDcntrlTimesTopic = timesTopic;
		mqttTopicsubscribe(WDcntrlTimesTopic.c_str(), 2); // Subscribe
	}
	void setWDUIcommandStateTopic(String UIcmdTopic)
	{
		WDUICmdTopic = UIcmdTopic;
		mqttTopicsubscribe(WDUICmdTopic.c_str(), 2); // Subscribe
	}
	void setWECntrlTimesTopic(String timesTopic)
	{
		WEcntrlTimesTopic = timesTopic;
		mqttTopicsubscribe(WEcntrlTimesTopic.c_str(), 2); // Subscribe
	}
	void setWEUIcommandStateTopic(String UIcmdTopic)
	{
		WEUICmdTopic = UIcmdTopic;
		mqttTopicsubscribe(WEUICmdTopic.c_str(), 2); // Subscribe
	}

	void setWDCntrlRunTimesStateTopic(String runtimeState)
	{
		WDCntrlRuntimeStateTopic = runtimeState;
	}
	void setWECntrlRunTimesStateTopic(String runtimeState)
	{
		WECntrlRuntimeStateTopic = runtimeState;
	}

	void setWDCntrlTimesReceived(bool ctr)
	{
		WDcntrlTimesReceived = ctr;
	}
	void setWECntrlTimesReceived(bool ctr)
	{
		WEcntrlTimesReceived = ctr;
	}
	void setRefreshID(String rID)
	{
		refreshID = rID;
	}
	void setCntrlName(String cID)
	{
		cntrlName = cID;
	}

	String getRefreshID()
	{
		return refreshID;
	}
	bool getWDCntrlTimesReceived()
	{
		return WDcntrlTimesReceived;
	}
	bool getWECntrlTimesReceived()
	{
		return WEcntrlTimesReceived;
	}
	String getWDCntrlTimesTopic()
	{
		return WDcntrlTimesTopic;
	}
	String getWECntrlTimesTopic()
	{
		return WEcntrlTimesTopic;
	}
	String getWDCntrlRunTimesStateTopic()
	{
		return WDCntrlRuntimeStateTopic;
	}
	String getWECntrlRunTimesStateTopic()
	{
		return WECntrlRuntimeStateTopic;
	}
	String getWDUIcommandStateTopic()
	{
		return WDUICmdTopic;
	}
	String getWEUIcommandStateTopic()
	{
		return WEUICmdTopic;
	}
	bool getWDCommandReceived()
	{
		return WDcommandReceived;
	}
	bool getWECommandReceived()
	{
		return WEcommandReceived;
	}
	int getWDHoldState()
	{
		return WDHoldState;
	}
	int getWEHoldState()
	{
		return WEHoldState;
	}
	//****************************************************************
	// Process any application specific inbound MQTT messages
	// Return False if none
	// Return true if an MQTT message was handled here
	//****************************************************************
	bool onMqttMessageCntrlExt(char *topic, char *payload, const AsyncMqttClientMessageProperties &properties, const size_t &len, const size_t &index, const size_t &total)
	{
		(void)payload;
		// char logString[MAX_LOGSTRING_LENGTH];

		char mqtt_payload[len + 1];
		mqtt_payload[len] = '\0';
		strncpy(mqtt_payload, payload, len);

		if (reporting == REPORT_DEBUG)
		{
			mqttLog("Received Topic and payload: ", true, true);
			mqttLog(topic, true, true);
			mqttLog(mqtt_payload, true, true);
		}
		
		/****************************************************
		 * Week days Times received
		 * array of three times zones : on,off,on,off,on,off
		 ****************************************************/
		if (strcmp(topic, getWDCntrlTimesTopic().c_str()) == 0)
		{
			mqttLog("WD Control times Received", true, true);
			processCrtlTimes(mqtt_payload, WDcntrlTimes, WDlcntrlTimes);

			setWDCntrlTimesReceived(true);
			return true;
		}
		/****************************************************
		 * Week End Times received
		 * array of three times zones : on,off,on,off,on,off
		 ****************************************************/
		else if (strcmp(topic, getWECntrlTimesTopic().c_str()) == 0)
		{
			mqttLog("WE Control times Received", true, true);
			processCrtlTimes(mqtt_payload, WEcntrlTimes, WElcntrlTimes);

			setWECntrlTimesReceived(true);
			return true;
		}
		/************************************************************************
		 * Weekday Command Control
		 * Message content:
		 * ON :  Set control permanently on. Ignore all time of day settings
		 * OFF:  Set control permanently off. Ignore all time of day settings
		 * ONOFF: open gates and allow to close normally
		 * NEXT: Move control setting to next time zone
		 * SET:  Set the heat times
		 ************************************************************************/
		else if (strcmp(topic, getWDUIcommandStateTopic().c_str()) == 0)
		{
			WDcommandReceived = true;
			if (coreServices.getWeekDayState() == true)
			{
				if (processCntrlMessage(mqtt_payload, "ON", "OFF", getWDUIcommandStateTopic().c_str()) == true)
				{
					Serial.print("ERROR: Unknown message - ");
					Serial.print(mqtt_payload);
					Serial.print(" - received for topic ");
					Serial.println(getWDUIcommandStateTopic().c_str());
				}
			}
			return true;
		}
		/************************************************************************
		 * Weekend Command Control
		 * Message content:
		 * ON :  Set control permanently on. Ignore all time of day settings
		 * OFF:  Set control permanently off. Ignore all time of day settings
		 * NEXT: Move control setting to next time zone
		 * SET:  Set the heat times
		 ************************************************************************/
		else if (strcmp(topic, getWEUIcommandStateTopic().c_str()) == 0)
		{
			WEcommandReceived = true;
			if (coreServices.getWeekDayState() == false) // false == weekend
			{
				if (processCntrlMessage(mqtt_payload, "ON", "OFF", getWEUIcommandStateTopic().c_str()) == true)
				{
					Serial.print("ERROR: Unknown message - ");
					Serial.print(mqtt_payload);
					Serial.print(" - received for topic ");
					Serial.println(getWEUIcommandStateTopic().c_str());
				}
			}
			return true;
		}
		else
		{
			//mqttLog("Unknown message Received Topic and payload: ", true, true);
			//mqttLog(topic, true, true);
			//mqttLog(mqtt_payload, true, true);
		
		   return false;
		}
	}

	//***********************************************************************************
	// inStr:   INPUT.  A string containing the times
	// ptr:     INPUT.  points to the array of ON and OFF times as characters
	// lptr:    OUTPUT. The function populates the array with ON and OFF times as integers
	//************************************************************************************
	void processCrtlTimes(char *inStr, char (&ptr)[6][10], int lptr[6])
	{
		/************************************************************************
		 * Parse the string containing the on and off times into a string array.
		 * e.g. 06:30,09:20,15:10,17:40,20:00,23:50 : This means
		 *
		 *  	[0] 06:30 - ON		Time Zone 1
		 * 		[1] 09:20 - OFF		Time Zone 1
		 * 		[2] 15:10 - ON		Time Zone 2
		 * 		[3] 17:40 - OFF		Time Zone 2
		 * 		[4] 20:00 - ON		Time Zone 3
		 * 		[5] 23:50 - OFF		Time Zone 3
		 *
		 ************************************************************************/
		int i = 0;
		char *pch;
		char timeString[10];
		char inStrLocal[255];  // local copy}
		memset(inStrLocal, '\0', sizeof(inStrLocal));
		strcpy(inStrLocal, inStr);
		mqttLog(inStrLocal, true, true);

		pch = strtok(inStrLocal, ",");
		while (pch != NULL)
		{
			if (pch != NULL)
			{
				char *output = &ptr[i][0];
				for (int i = 0, j = 0; i < 4; i++, j++) // Evaluate each character in the input
				{
					if (pch[i] != ':')		// If the character is not a space
						output[j] = pch[i]; // Copy that character to the output char[]
					else
						j--; // If it is a space then do not increment the output index (j), the next non-space will be entered at the current index
				}
				output[4] = '\0';

				memset(timeString, 0, sizeof(timeString));
				sprintf(timeString, "%s", output);
				//sprintf(timeString, "%s", &ptr[i][0]);

				//mqttLog(timeString, true, true);

				lptr[i] = atoi(timeString); // Store integer value of time
											// comment out
											// mqttLog(timeString,true);
			}
			pch = strtok(NULL, ",");
			i++;
		}
	}

	/************************************************************
	 * Process lighting control ON, OFF, SET and NEXT messages
	 * returns: false = ok
	 * 			true  = fail
	 ************************************************************/
	bool processCntrlMessage(char *mqttMessage, const char *onMessage, const char *offMessage, const char *commandTopic)
	{
		if (strcmp(mqttMessage, "ON") == 0)
		{

			if (coreServices.getWeekDayState() == true)
			{
				setWDRunMode(ONMODE);
				//mqttClient.publish(getWDCntrlRunTimesStateTopic().c_str(), 0, false, "ON"); 
				app_WD_on(cntrlObjRef);
			}
			else
			{
				setWERunMode(ONMODE);
				//mqttClient.publish(getWECntrlRunTimesStateTopic().c_str(), 0, false, "ON"); 
				app_WE_on(cntrlObjRef);
			}
		}
		else if (strcmp(mqttMessage, "OFF") == 0)
		{
			//*rm = OFFMODE;
			if (coreServices.getWeekDayState() == true)
			{
				setWDRunMode(OFFMODE);
				//mqttClient.publish(getWDCntrlRunTimesStateTopic().c_str(), 0, true, "OFF"); // FIXTHIS WD or WE
				app_WD_off(cntrlObjRef);
			}
			else
			{
				setWERunMode(OFFMODE);
				//mqttClient.publish(getWECntrlRunTimesStateTopic().c_str(), 0, true, "OFF"); // FIXTHIS WD or WE
				app_WE_off(cntrlObjRef);
			}
		}
		else if (strcmp(mqttMessage, "ONOFF") == 0)
		{
			//mqttClient.publish(getWDCntrlRunTimesStateTopic().c_str(), 0, true, "ON");	// FIXTHIS WD or WE
			app_WD_on(cntrlObjRef);														// FIXTHIS WD or WE
			delay(5000);																// FIX this
			//mqttClient.publish(getWDCntrlRunTimesStateTopic().c_str(), 0, true, "OFF"); // FIXTHIS WD or WE
			app_WD_off(cntrlObjRef);														// FIXTHIS WD or WE

			// setWDRunMode(AUTOMODE); // FIX THIS : why am I doing this? leave as is
		}
		else if (strcmp(mqttMessage, "NEXT") == 0)
		{
			if (coreServices.getWeekDayState() == true)
			{
				setWDRunMode(NEXTMODE);
				setWDSwitchBack(SBOFF);													   		// Switch back to AUTOMODE when Time of Day is next OFF (don't switch back when this zone ends)
				//mqttClient.publish(getWDCntrlRunTimesStateTopic().c_str(), 0, true, "ON"); 	// FIXTHIS WD or WE
				if (onORoff() == false)
				{
					setWDHoldState(1);									// the state to hold while in NEXTMDDE
					app_WD_on(cntrlObjRef);
				}													  	// Set on because we want ON until the end of the next OFF
				else
				{
					setWDHoldState(0);									// the state to hold while in NEXTMDDE
					app_WD_off(cntrlObjRef);							// Set off because we want ON until the end of the next OFF
				}
			}
			else
			{
				setWERunMode(NEXTMODE);
				setWESwitchBack(SBOFF);													   		// Switch back to AUTOMODE when Time of Day is next ON (don't switch back when this zone ends)
				//mqttClient.publish(getWECntrlRunTimesStateTopic().c_str(), 0, true, "ON"); // FIXTHIS WD or WE
				if (onORoff() == false)
				{ 
					setWEHoldState(1);	
					app_WE_on(cntrlObjRef);													  	// Set off because we want OFF until the end of the next OFF
				}
				else
				{
					setWEHoldState(0);	
					app_WE_off(cntrlObjRef);
				}
			}
		}
		else if (strcmp(mqttMessage, "SET") == 0)
		{

			mqttLog("processCntrlMessage: SET received.", true, true);

			// IF pressed SET then check the ON Close time and sent the appropriate message
			if (coreServices.getWeekDayState() == true)
			{
				setWDRunMode(AUTOMODE);
				setWDHoldState(9);
				if (onORoff() == true)
				{
					//mqttClient.publish(getWDCntrlRunTimesStateTopic().c_str(), 0, true, "ON"); // FIXTHIS WD or WE
					app_WD_on(cntrlObjRef);
				}
				else
				{
					//mqttClient.publish(getWDCntrlRunTimesStateTopic().c_str(), 0, true, "OFF"); // FIXTHIS WD or WE
					app_WD_off(cntrlObjRef);
				}
			}
			else
			{
				setWERunMode(AUTOMODE);
				setWEHoldState(9);
				if (onORoff() == true)
				{
					//mqttClient.publish(getWECntrlRunTimesStateTopic().c_str(), 0, true, "ON"); // FIXTHIS WD or WE
					app_WE_on(cntrlObjRef);
				}
				else
				{
					//mqttClient.publish(getWECntrlRunTimesStateTopic().c_str(), 0, true, "OFF"); // FIXTHIS WD or WE
					app_WE_off(cntrlObjRef);
				}
			}
		}
		else
		{
			processCntrlMessageApp_Ext(mqttMessage, onMessage, offMessage, commandTopic);
			return true;
		}
		processCntrlMessageApp_Ext(mqttMessage, onMessage, offMessage, commandTopic);
		//mqttLog("Returning from processCntrlMessage: ", true, true);
		return false;
	}

	/***********************************************************************
	 * Determines if control should be ON or OFF based on current zone times
	 * Uses and updates global variables:
	 *    weekDay,
	 *    lcntrlTimes,
	 *    lcntrlTimesWE,
	 *    Zone,
	 *    ZoneWE.
	 *
	 * Returns true to switch ON, false to switch OFF
	 ************************************************************************/
	bool onORoff()
	{

		// char logString[MAX_LOGSTRING_LENGTH];
		//  debugPrint();
		//mqttLog("processCntrlMessage: onORoff checking", true, true);

		if (coreServices.getWeekDayState() == true)
			setWDZone(ZONEGAP); // not in a zone
		else
			setWEZone(ZONEGAP);

		bool state = false;

		if (readyCheck() == true) // All conditions to start are met
		{
			/****************************************************************
			 * Work out from the array of longs if the control is On or OFF
			 ****************************************************************/
			//Serial.print("PS ZONE CONFIG: ");

			//for (int i = 0, j = 0; i < 6; i++, j++)
			int j = 1;
			for (int i = 0; i < 6; i++)
			{
				// Serial.print (", Zone: ");
				// Serial.print ((int) i - j);
				// Serial.print ("[");
				// Serial.print ( lcntrlTimes[i]);
				// Serial.print (",");
				// Serial.print ( lcntrlTimes[i+1]);
				// Serial.print ("] ");

				// memset(logString,0, sizeof logString);
				// sprintf(logString, "%s,%s,%s,%s[%i: %i: %i]", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "ohTimenow: ", ohTimenow, lcntrlTimesWD[i], lcntrlTimesWE[i]);
				// mqttLog(logString,true, true);

				if (coreServices.getWeekDayState() == true)
				{
					if (ohTimenow >= WDlcntrlTimes[i] && ohTimenow < WDlcntrlTimes[i + 1])
					{
						state = true; // Switch ON
						setWDZone(j); // Update which zone we are in (there maybe overlapping time zones)
						break;
					}
				}
				else
				{
					if (ohTimenow >= WElcntrlTimes[i] && ohTimenow < WElcntrlTimes[i + 1])
					{
						state = true; // Switch ON
						setWEZone(j); // Update which zone we are in (there maybe overlapping time zones)
						break;
					}
				}
				i++;
				j++;
			}
		}
		return state;
	}

	//**********************************************************************
	// Controller TOD Porocessing
	// check current time against configutation and decide what to do.
	// Send appropriate MQTT command for each control.
	// Run this everytime the TOD changes
	// If TOD event is not received then nothing happens
	//**********************************************************************
	void processCntrlTOD_Ext()
	{
		bool onORoffstate = false;
		int wdzone = 9;
		int wezone = 9;

		String msg = cntrlName + " Contoller Processing TOD";
		mqttLog(msg.c_str(), true, true);

		char logString[MAX_LOGSTRING_LENGTH];

		// Check all WD and WE times have been received before doing anything
		if (runTimeReceivedCheck() == true)
		{
			if (getWDRunMode() == NEXTMODE && coreServices.getWeekDayState() == true)
			{
				onORoffstate = onORoff();
				wdzone = getWDZone();

				String logRecord = "SwitchBack: " + (String) getWDSwitchBack() + " Zone: " + (String) wdzone + " onOroff: " + (String)onORoffstate   + " Hold: " + getWDHoldState();
				mqttLog(logRecord.c_str(), true, true);


				if (getWDHoldState() == onORoffstate)
				{
					setWDSwitchBack(SBON); // SBON means switch back to normal operation at the end of a gap period
					setWDRunMode(AUTOMODE);
					app_WD_auto(cntrlObjRef);
				}
			}
			else if (getWERunMode() == NEXTMODE && coreServices.getWeekDayState() == false)
			{
				onORoffstate = onORoff();
				wezone = getWEZone();			

				String logRecord = "SwitchBack: " + (String) getWESwitchBack() + " Zone: " + (String) wezone + " onOroff: " + (String)onORoffstate + " Hold: " + getWEHoldState();
				mqttLog(logRecord.c_str(), true, true);

				if (getWEHoldState() == onORoffstate)
				{
					setWESwitchBack(SBON); // SBON means switch back to normal operation at the end of a gap period
					setWERunMode(AUTOMODE);
					app_WE_auto(cntrlObjRef);
				}
			}
			else if (getWDRunMode() == AUTOMODE && coreServices.getWeekDayState() == true)
			{
				if (onORoff() == true)
				{
					// trigger Application let it know that the Contoller in an on period.
					// Let the app decide what to do.
					app_WD_on(cntrlObjRef);
				}
				else
				{
					app_WD_off(cntrlObjRef);
				}
			}
			else if (getWERunMode() == AUTOMODE && coreServices.getWeekDayState() == false)
			{
				if (onORoff() == true)
				{
					app_WE_on(cntrlObjRef);
				}
				else
				{
					app_WE_off(cntrlObjRef);
				}
			}
			// FIXTHIS  logic not sound - I think do each individually
			else if (getWDRunMode() == ONMODE || getWDRunMode() == ONMODE)
			{
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Permanently ON");
				mqttLog(logString, true, true);
			}
			else if (getWERunMode() == OFFMODE || getWERunMode() == OFFMODE)
			{
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Permanently OFF");
				mqttLog(logString, true, true);
			}
			else
			{
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Unknown running mode ");
				mqttLog(logString, true, true);
			}
		}
	}
	/*
	 * Check that the initialisation data all been received before processing
	 * Returns True if all checks have been received
	 */
	bool readyCheck()
	{
		if (ohTODReceived == true && getWDCntrlTimesReceived() == true && getWECntrlTimesReceived() == true)
		{
			// mqttLog("readyCheck = true", true, true);
			return true;
		}
		else
		{
			return false;
		}
	}

	// Triggerd by xxx ticker
	bool runTimeReceivedCheck()
	{
		char logString[MAX_LOGSTRING_LENGTH];

		if (getWDCntrlTimesReceived() == false || getWECntrlTimesReceived() == false || WDcommandReceived == false || WEcommandReceived == false || ohTODReceived == false)
		{
			memset(logString, 0, sizeof logString);
			sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Waiting for all initialisation messages to be received");
			mqttLog(logString, true, true);

			if (ohTODReceived == false)
			{
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Time of Day not yet received");
				mqttLog(logString, true, true);
			}
			if (getWDCntrlTimesReceived() == false)
			{
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Weekday control times not yet received");
				mqttLog(logString, true, true);
			}
			if (getWECntrlTimesReceived() == false)
			{
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Weekend control times not yet received");
				mqttLog(logString, true, true);
			}
			if (WDcommandReceived == false)  // && coreServices.getWeekDayState() == true)
			{
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Weekday operating mode not yet received");
				mqttLog(logString, true, true);
			}
			if (WEcommandReceived == false) // && coreServices.getWeekDayState() == false)
			{
				memset(logString, 0, sizeof logString);
				sprintf(logString, "%s,%s,%s,%s", ntptod, espDevice.getType().c_str(), espDevice.getName().c_str(), "Weekend operating mode not yet received");
				mqttLog(logString, true, true);
			}
			// FIXTHIS ... WD and WE python returns both
			mqttClient.publish(oh3StateIOTRefresh, 0, true, getRefreshID().c_str());
			return false;
		}
		else
			return true;
	}

	//************************************************************************
	// Write out over telnet session and application specific infomation
	// FIXTHIS: Move to utilities
	//***********************************************************************
	void telnet_extension_1(char c)
	{
		// char logString[MAX_LOGSTRING_LENGTH];
		// memset(logString, 0, sizeof logString);
		// sprintf(logString, "%s%d\n\r", "Sensor Value:\t", sensorValue);
		// printTelnet((String)logString);

		String wd;
		String rmWD, rmWE;
		char stringOfTimesWD[70];
		char stringOfTimesWE[70];
		// get weekday
		wd = "Week Day";
		if (coreServices.getWeekDayState() == false)
			wd = "Weekend";

		// Get on, off times
		sprintf(stringOfTimesWD, "%s,%s,%s,%s,%s,%s", WDcntrlTimes[0], WDcntrlTimes[1], WDcntrlTimes[2], WDcntrlTimes[3], WDcntrlTimes[4], WDcntrlTimes[5]);
		sprintf(stringOfTimesWE, "%s,%s,%s,%s,%s,%s", WEcntrlTimes[0], WEcntrlTimes[1], WEcntrlTimes[2], WEcntrlTimes[3], WEcntrlTimes[4], WEcntrlTimes[5]);

		// get Run Mode
		if (getWDRunMode() == AUTOMODE)
			rmWD = "AUTOMODE";
		else if (getWDRunMode() == NEXTMODE)
			rmWD = "NEXTMODE";
		else if (getWDRunMode() == ONMODE)
			rmWD = "ONMODE";
		else if (getWDRunMode() == OFFMODE)
			rmWD = "OFFMODE";
		else
			rmWD = "UNKNOWN";

		if (getWERunMode() == AUTOMODE)
			rmWE = "AUTOMODE";
		else if (getWERunMode() == NEXTMODE)
			rmWE = "NEXTMODE";
		else if (getWERunMode() == ONMODE)
			rmWE = "ONMODE";
		else if (getWERunMode() == OFFMODE)
			rmWE = "OFFMODE";
		else
			rmWE = "UNKNOWN";

		// Print out on Telnet terminal
		char logString[MAX_LOGSTRING_LENGTH];
		// memset(logString, 0, sizeof logString);
		sprintf(logString, "%s%s\r", "Cntlr Name:\t", cntrlName.c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "Refresh ID:\t", refreshID.c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "Week Day:\t", wd.c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WD Times:\t", stringOfTimesWD);
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WE Times:\t", stringOfTimesWE);
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WD Run Mode:\t", rmWD.c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WE Run Mode:\t", rmWE.c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%i\r", "WD SB Mode:\t", getWDSwitchBack());
		printTelnet((String)logString);
		sprintf(logString, "%s%i\r", "WE SB Mode:\t", getWESwitchBack());
		printTelnet((String)logString);
		sprintf(logString, "%s%s%i%s%i%s%i%s%i\r", "Cntrl config:\t", "WDT:", getWDCntrlTimesReceived(), " WET:", getWECntrlTimesReceived(), " WDC:", getWDCommandReceived(), " WEC:", getWECommandReceived());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WD CT Topic:\t", getWDCntrlTimesTopic().c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WE CT Topic:\t", getWECntrlTimesTopic().c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WD Cmd Topic:\t", getWDUIcommandStateTopic().c_str());
		printTelnet((String)logString);
		sprintf(logString, "%s%s\r", "WE Cmd Topic:\t", getWEUIcommandStateTopic().c_str());
		printTelnet((String)logString);
		printTelnet((String)" ");
	}
};





#endif