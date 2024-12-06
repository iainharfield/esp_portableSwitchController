#include <WString.h>

#define WDCntlTimes  	"/house/cntrl/portable-switch/wd-control-times" 		// Times received from either UI or Python app
#define WECntlTimes  	"/house/cntrl/portable-switch/we-control-times" 		// Times received from either UI or MySQL via Python app
#define runtimeState    "/house/cntrl/portable-switch/runtime-state" 	 		// published state: ON, OFF, and AUTO
#define LightState      "/house/cntrl/portable-switch/light-state"              // ON or OFF
#define WDUICmdState 	"/house/cntrl/portable-switch/wd-command"		 		// UI Button press received
#define WEUICmdState 	"/house/cntrl/portable-switch/we-command"		 		// UI Button press received
#define RefreshID		"PS01"		

#define startUpMessage  "\nStarting Portable Switch Controller on "

String deviceName   = "portable-switch";
String deviceType   = "CNTRL";
String app_id		= "PSC";		// configure

const char *oh3CommandTrigger   = "/house/cntrl/outside-lights-front/pir-command";	    // Event fron the PIR detector (front porch: PIRON or PIROFF
const char *oh3StateManual		= "/house/cntrl/portable-switch/manual-state";	 // 	Status of the Manual control switch control MAN or AUTO