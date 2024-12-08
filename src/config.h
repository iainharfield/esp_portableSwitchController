#include <WString.h>
#define PS01
//#define PS02

#ifdef PS01

#define WDCntlTimes  	"/house/cntrl/portable-switch/wd-control-times" 		// Times received from either UI or Python app
#define WECntlTimes  	"/house/cntrl/portable-switch/we-control-times" 		// Times received from either UI or MySQL via Python app
#define runtimeState    "/house/cntrl/portable-switch/runtime-state" 	 		// published state: ON, OFF, and AUTO
#define LightState      "/house/cntrl/portable-switch/light-state"              // ON or OFF
#define WDUICmdState 	"/house/cntrl/portable-switch/wd-command"		 		// UI Button press received
#define WEUICmdState 	"/house/cntrl/portable-switch/we-command"		 		// UI Button press received
#define RefreshID		"PS01"		

#define startUpMessage  "\nStarting Portable Switch 01 Controller on "

String deviceName   = "portable-switch-01";
String deviceType   = "CNTRL";
String app_id		= "PSC01";		// configure

const char *oh3CommandTrigger   = "/house/cntrl/outside-lights-front/pir-command";	    // Event fron the PIR detector (front porch: PIRON or PIROFF
const char *oh3StateManual		= "/house/cntrl/portable-switch/manual-state";	 // 	Status of the Manual control switch control MAN or AUTO

#endif

#ifdef PS02

#define WDCntlTimes  	"/house/cntrl/portable-switch-02/wd-control-times" 		// Times received from either UI or Python app
#define WECntlTimes  	"/house/cntrl/portable-switch-02/we-control-times" 		// Times received from either UI or MySQL via Python app
#define runtimeState    "/house/cntrl/portable-switch-02/runtime-state" 	    // published state: ON, OFF, and AUTO
#define LightState      "/house/cntrl/portable-switch-02/light-state"           // ON or OFF
#define WDUICmdState 	"/house/cntrl/portable-switch-02/wd-command"		    // UI Button press received
#define WEUICmdState 	"/house/cntrl/portable-switch-02/we-command"		 	// UI Button press received
#define RefreshID		"PS02"		

#define startUpMessage  "\nStarting Portable Switch 02 Controller on "

String deviceName   = "portable-switch-02";
String deviceType   = "CNTRL";
String app_id		= "PSC02";		// configure

const char *oh3CommandTrigger   = "/house/cntrl/outside-lights-front/pir-command";	    // Event fron the PIR detector (front porch: PIRON or PIROFF
const char *oh3StateManual		= "/house/cntrl/portable-switch-02/manual-state";	 // 	Status of the Manual control switch control MAN or AUTO

#endif
