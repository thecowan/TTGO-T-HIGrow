void goToDeepSleep()
{
  int sleepTime = TIME_TO_SLEEP;
  if (config.batteryChargeState.equals("charging")) {
    sleepTime = TIME_TO_SLEEP_IF_CHARGING;
  }
  Serial.print("Going to sleep for ");
  Serial.print(sleepTime);
  Serial.println(" seconds");
  if (logging) {
    writeFile(SPIFFS, "/error.log", "Going to sleep for 10800 seconds \n");
  }

  if (DELAY_ONLY) {
    Serial.print("(Delaying only, no sleep)");
    delay(sleepTime * mS_TO_S_FACTOR);
    return;
  }

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();

  // Configure the timer to wake us up!
  esp_sleep_enable_timer_wakeup(sleepTime * uS_TO_S_FACTOR);

  // Testpurposes
  //esp_sleep_enable_timer_wakeup(10 * uS_TO_S_FACTOR);

  if (logging)
  {
    writeFile(SPIFFS, "/error.log", "Going to deep sleep \n \n \n");
  }

  // Go to sleep! Zzzz
  esp_deep_sleep_start();
}
void goToDeepSleepFiveMinutes()
{
  Serial.print("Going to sleep... ");
  Serial.print("300");
  Serial.println(" sekunder");
  if (logging) {
    writeFile(SPIFFS, "/error.log", "Going to sleep for 300 seconds \n");
  }

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();

  adc_power_off();
  esp_wifi_stop();
  esp_bt_controller_disable();

  // Configure the timer to wake us up!
  ++sleep5no;
  esp_sleep_enable_timer_wakeup(300 * uS_TO_S_FACTOR);

  // Go to sleep! Zzzz
  esp_deep_sleep_start();
}
