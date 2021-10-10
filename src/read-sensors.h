// READ Sensors

// READ Salt
// I am not quite sure how to read and use this number. I know that when put in water wich a DH value of 26, it gives a high number, but what it is and how to use ??????
uint32_t readSalt()
{
  uint8_t samples = 120;
  uint32_t humi = 0;
  uint16_t array[120];

  for (int i = 0; i < samples; i++)
  {
    array[i] = analogRead(SALT_PIN);
  //  Serial.print("Read salt pin : ");

  //  Serial.println(array[i]);
    delay(2);
  }
  std::sort(array, array + samples);
  for (int i = 0; i < samples; i++)
  {
    if (i == 0 || i == samples - 1)
      continue;
    humi += array[i];
  }
  humi /= samples - 2;
  return humi;
}

// READ Soil
uint16_t readSoil()
{
  uint16_t soil = analogRead(SOIL_PIN);
  return soil;
}

uint16_t mapSoil(uint16_t rawSoil)
{
  uint16_t mapped = map(rawSoil, soil_min, soil_max, 100, 0);
  String error = "";
  if (rawSoil < soil_min) {
    mapped = 100;
    error = " (max); value outside range, consider lowering soil_min";
  } else if (rawSoil > soil_max) {
    mapped = 0;
    error = " (min); value outside range, consider raising soil_max";
  }
  Serial.printf("Raw soil data read before mapping: %i, mapping to range %i (wet) - %i (dry); mapped to %i%s\n", rawSoil, soil_min, soil_max, mapped, error.c_str());
  return mapped;
}

float readSoilTemp()
{
  float temp;
  // READ Soil Temperature
  if (USE_18B20_TEMP_SENSOR)
  {
    //Single data stream upload
    temp = temp18B20.temp();
  }
  else
  {
    temp = 0.00;
  }
  return temp;
}

// READ Battery
float readBattery()
{
  int vref = 1100;
  uint16_t volt = analogRead(BAT_ADC);
  Serial.printf("Voltage reading (raw): %i\n", volt);
  config.rawBatteryVoltage = volt;
  float battery_voltage = ((float)volt / 4095.0) * 2.0 * 3.3 * (vref) / 1000;
  Serial.printf("Calculated Battery Voltage: %f\n", battery_voltage);
  config.batteryVoltage = battery_voltage;
  battery_voltage = battery_voltage * 100;
  return map(battery_voltage, 416, 290, 100, 0);
}