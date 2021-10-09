// *******************************************************************************************************************************
// START userdefined data
// *******************************************************************************************************************************
#include <Arduino.h>

// *******************************************************************************************************************************
// *** PLANT-SPECIFIC - you'll want to change these for every plant
// *******************************************************************************************************************************

// Give the sensor a friendly plant name
String plant_name = "Padron_1";

// If using the Greenhouse automatic watering repo, then assign a waterValveNo to the plant.
int plantValveNo = 1;

// Whether to auto-calibrate the device as extreme observations are recorded. You most likely don't want to change this, but YOU ARE
// STRONGLY RECOMMENDED to quickly calibrate each device before deploying in your garden; see instructions at the end of this file.
// If you change this to false, you WILL need to update the defaults at the end of this file by hand.
const bool auto_calibrate = true;

// *******************************************************************************************************************************
// *** WIFI/MQTT setup - you likely want the same values for all your devices
// *******************************************************************************************************************************

String ssidArr[] = {"Enterprise-pro", "Enterprise_EXT", "Enterprise_EXTN", "Enterprise" };
int ssidArrNo = 4;
const char* password = "password";

const char broker[] = "192.168.1.64";
int        port     = 1883;
const char mqttuser[] = ""; //add eventual mqtt username
const char mqttpass[] = ""; //add eventual mqtt password

// *******************************************************************************************************************************
// *** DEVICE HARDWARE CONFIGURATION - different for every type of device you buy
// *******************************************************************************************************************************

// Select DHT type on the module - supported are DHT11, DHT12, DHT22 -- Even if you do not have one, you have to uncomment one, as compilation else will fail.
//#define DHT_TYPE DHT11
#define DHT_TYPE DHT12
//#define DHT_TYPE DHT22

// Set to true if you have a DHT sensor on the board, and false if not
bool dht_found = false;

// Select if 18B20 soil temp sensor available, if available -->> set to true
const bool USE_18B20_TEMP_SENSOR = false;


// *******************************************************************************************************************************
// *** ADVANCED CHANGES - you can change these if you want, but defaults are sensible and changes likely unnecessary unless debugging
// *******************************************************************************************************************************
const char* ntpServer = "pool.ntp.org";

// Turn logging on/off - turn read logfile on/off, turn delete logfile on/off ---> default is false for all 3, otherwise it can cause battery drainage.
const bool  logging = false;
const bool  readLogfile = false;
const bool  deleteLogfile = false;

// Device configuration and name setting
const String device_name = "Tgrow_HIGrow"; // Can be changed, but not necessary, as it will give no added value.

#define TIME_TO_SLEEP  10800          //Time ESP32 will go to sleep (in seconds)
#define TIME_TO_SLEEP_IF_CHARGING 900 //Time ESP32 will go to sleep (in seconds) when connected to charger
#define DELAY_ONLY     false          // If true, device will stay awake, idling between loops. For debugging purposes only: will drain battery!

// Salt/Fertilizer recommandation break points. You can change these according to your own calibration measurements.
int fertil_needed = 200;
int fertil_low = 201;
int fertil_opt = 251;
int fertil_high = 351;

// *******************************************************************************************************************************
// *** CALIBRATION INFORMATION - auto-calibrate each device before deploying!
// *******************************************************************************************************************************

// The firmware incorporates self-calibration. The default values below (used on first boot) are deliberately too tight, and will be adjusted based on observed extremes.
//
// To calibrate the device, first boot it when the unit is sitting completely dry on the table (zero humidity); you should see a line in the logs like:
//     No soil calibration info saved; using defaults of 1700:3000
// followed later by:
//     Raw soil data read before mapping: 3415 [...] value outside range, will raise soil_max
// and finally by
//     Spotted soil values outside calibration; saving
// at the end of the cycle. This means that the new calibration extremes of e.g. 1700:3415 have been saved (you may wish to push the 'wake' button one or
// two times to get another couple of reads in case of variations/noise in the readings).
//
// You will then repeat the process, with the blade of the sensor submerged in water, to get the lowest ('wettest') reading calibrated also.
//
// Typical values might be around 1450:3350 after calibration, but can vary. A safeguard exists where values won't be updated outside 'ridiculous' extremes
// (500-4000); if you have a device which actually reports outside this, you'll need to turn auto-calibration off at the top of this file, and update 
// soil_min_default and soil_max_default below based on what you observe; they will then be the only values used.

// Soil defaults - NO NEED TO CHANGE THESE if using auto-calibration.
const int soil_min_default = 1700;
const int soil_max_default = 3000;

// *******************************************************************************************************************************
// END userdefined data
// *******************************************************************************************************************************
