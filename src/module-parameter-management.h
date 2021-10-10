// Initiate SPIFFS and Mount file system
if (!SPIFFS.begin(true))
{
  Serial.println("An Error has occurred while mounting SPIFFS");
  if (logging)
  {
    writeFile(SPIFFS, "/error.log", "An Error has occurred while mounting SPIFFS \n");
  }
  return;
}
if (logging)
{
  writeFile(SPIFFS, "/error.log", "Void Setup \n");
}

listDir(SPIFFS, "/", 0);

if (wipeSavedState) {
  Serial.println("Erasing soil calibration file");
  SPIFFS.remove("/soilcalib.conf");
  Serial.println("Erasing battery charge info");
  SPIFFS.remove("/batinfo.conf");
}

if (logging)
{
  writeFile(SPIFFS, "/error.log", "After listDir \n");
}

if (readLogfile)
{
  // Now we start reading the files..
  readFile(SPIFFS, "/error.log");
  Serial.println("Here STARTS the logging info:");
  Serial.println(readString);
  Serial.println("Here ENDS the logging info:");
}

if (deleteLogfile)
{
  SPIFFS.remove("/error.log");
}

pinMode(led, OUTPUT);
digitalWrite(led, 0);
if (logging)
{
  writeFile(SPIFFS, "/error.log", "Before Start WIFI \n");
}

read_batt_info();
read_soil_calibration();