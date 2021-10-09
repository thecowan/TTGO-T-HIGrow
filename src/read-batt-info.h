#include <Arduino.h>
// Read battery charging info
void read_batt_info()
{
    readFile(SPIFFS, "/batinfo.conf");
    Serial.println("Last charge info (epoch~date) from stored file:");
    Serial.println(readString);

    battChargeEpoc = getValue(readString, '~', 0);
    config.batchargeDate = getValue(readString, '~', 1);

    readString = "";
}