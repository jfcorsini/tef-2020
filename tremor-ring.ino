#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "Adafruit_Si7021.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
Adafruit_Si7021 sensor = Adafruit_Si7021();

char auth[] = "";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

void writeToBlynk(float data, float virtualPin)
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(virtualPin, data);
}

void setup(void)
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  /* Initialise the sensor */
  if (!accel.begin() || !sensor.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, there is some problems with some sensor");
    while (true);
  }

  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);
  // accel.setRange(ADXL345_RANGE_8_G);
  // accel.setRange(ADXL345_RANGE_4_G);
  // accel.setRange(ADXL345_RANGE_2_G);
}

void loop(void)
{

  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");
  Serial.print("Humidity: ");Serial.print(sensor.readHumidity(), 2); Serial.print("  ");
  Serial.print("Temperature: ");Serial.print(sensor.readTemperature(), 2); Serial.print("  ");
  Serial.println("");
  delay(100);

  Blynk.run();
  writeToBlynk(event.acceleration.x, V3);
  writeToBlynk(event.acceleration.y, V1);
  writeToBlynk(event.acceleration.z, V2);
}
