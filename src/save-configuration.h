// Allocate a  JsonDocument
void saveConfiguration(const Config & config) {

  //  Serial.println(WiFi.macAddress());
  //  String stringMAC = WiFi.macAddress();
  //  stringMAC.replace(':', '_');

  byte mac[6];
  WiFi.macAddress(mac);

  //  String chipId = String(mac[0], HEX) + String(mac[1], HEX) + String(mac[2], HEX) + String(mac[3], HEX) + String(mac[4], HEX) + String(mac[5], HEX);
  String chipId = "";
  String HEXcheck = "";
  for (int i = 0; i <= 5; i++) {
    HEXcheck = String(mac[i], HEX);
    if (HEXcheck.length() == 1) {
      chipId = chipId + "0" + String(mac[i], HEX);
    } else {
      chipId = chipId + String(mac[i], HEX);
    }
  }
  const String topicStr = device_name + "/" + chipId;
  const char* topic = topicStr.c_str();
  Serial.printf("Publishing to topic %s via ssid %s\n", topic, ssid);

  StaticJsonDocument<1536> doc;
  // Set the values in the document
  // Device changes according to device placement
  JsonObject root = doc.to<JsonObject>();

  JsonObject plant = root.createNestedObject("plant");

  plant[device_name] = chipId;
  plant["sensor_name"] = plant_name;
  plant["last_updated"] = config.dateTime;
  plant["valve_number"] = plantValveNo;

  plant["lux"] = config.lux;
  plant["temperature"] = config.temperature;
  plant["humidity"] = config.humidity;
  plant["pressure"] = config.pressure;

  plant["raw_soil_moisture"] = config.rawSoilMoisture;
  plant["soil_moisture"] = config.soilMoisture;

  plant["soil_temperature"] = config.soilTemperature;

  plant["raw_soil_conductivity"] = config.rawSoilConductivity;
  plant["soil_conductivity"] = config.soilConductivity;
  plant["salt_advice"] = config.saltAdvice;

  plant["raw_battery_voltage"] = config.rawBatteryVoltage;
  plant["battery_voltage"] = config.batteryVoltage;
  plant["battery_percentage"] = config.batteryPercentage;
  plant["battery_charge_state"] = config.batteryChargeState;
  plant["battery_last_charged"] = config.batteryLastCharged;

  plant["boot_count"] = config.bootCount;
  plant["sleep_5_count"] = config.sleep5Count;
  plant["release_version"] = config.release;

  plant["wifi_ssid"] = WiFi.SSID();
  plant["wifi_rssi"] = WiFi.RSSI();

  // Send to mqtt
  char buffer[1536];
  serializeJson(doc, buffer);


  Serial.print("Sending message to topic: ");
  if (logging) {
    writeFile(SPIFFS, "/error.log", "Sending message to topic: \n");
  }

  Serial.println(buffer);

  // Connect to mqtt broker
  Serial.print("Attempting to connect to the MQTT broker: ");
  if (logging) {
    writeFile(SPIFFS, "/error.log", "Attempting to connect to the MQTT broker! \n");
  }

  Serial.println(broker);
  mqttClient.setServer(broker, 1883);

  if (!mqttClient.connect(broker, mqttuser, mqttpass)) {
    if (logging) {
      writeFile(SPIFFS, "/error.log", "MQTT connection failed! \n");
    }

    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.state());
    goToDeepSleepFiveMinutes();
  }

  if (logging) {
    writeFile(SPIFFS, "/error.log", "You're connected to the MQTT broker! \n");
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();

  bool retained = true;

  if (mqttClient.publish(topic, buffer, retained)) {
    Serial.println("Message published successfully");
  } else {
    Serial.println("Error in Message, not published");
    goToDeepSleepFiveMinutes();
  }
  Serial.println();
}

