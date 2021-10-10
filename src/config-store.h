#include <Arduino.h>
// Read battery charging info
void read_batt_info()
{
    readFile(SPIFFS, "/batinfo.conf");
    Serial.printf("Last charge info (epoch~date) from stored file: %s\n", readString.c_str());
    config.batteryLastCharged = readString;
    readString = "";
}

void read_soil_calibration()
{
  if (!auto_calibrate) {
    Serial.printf("Auto-calibration turned off; using hard-coded defaults of %i:%i\n", soil_min_default, soil_max_default);
    soil_max = soil_max_default;
    soil_min = soil_min_default;
    return;
  }
  readFile(SPIFFS, "/soilcalib.conf");
  if (readString.isEmpty()) {
    Serial.printf("No soil calibration info saved; using defaults of %i:%i\n", soil_min_default, soil_max_default);
    soil_max = soil_max_default;
    soil_min = soil_min_default;
  } else {
    Serial.printf("Read previous calibration info from file: %s\n", readString.c_str());
    String xval = getValue(readString, ':', 0);
    String yval = getValue(readString, ':', 1);

    soil_min = xval.toInt();
    soil_max = yval.toInt();
  }
  readString = "";
}

void write_soil_calibration()
{
  SPIFFS.remove("/soilcalib.conf");
  String soil_write_str = String(soil_min) + ":" + String(soil_max);
  const char *soil_write = soil_write_str.c_str();
  writeFile(SPIFFS, "/soilcalib.conf", soil_write);
}