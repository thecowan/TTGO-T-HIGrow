#include <Arduino.h>
// Read battery charging info
void read_batt_info()
{
    readFile(SPIFFS, "/batinfo.conf");
    Serial.printf("Last charge info (epoch~date) from stored file: %s\n", readString.c_str());

    battChargeEpoc = getValue(readString, '~', 0);
    config.batchargeDate = getValue(readString, '~', 1);

    readString = "";
}