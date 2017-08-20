/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 by Daniel Eichhorn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include "Arduino.h"
#include "SH1106.h"
#include "WiFi.h"
#include "DHT.h"

#define D1 5
#define D2 4

#define DHTPIN 0     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors

DHT dht(DHTPIN, DHTTYPE);

SH1106 display(0x3c, D1, D2);

//const char* ssid     = "your-ssid";
//const char* password = "your-password";
#include "password.h"  //put your password and ssid on lib/something/password.h and 
                       // *DON'T* commit the file

void setup() {
  Serial.begin(9600);
  delay(5000);
  Serial.println();
  Serial.println();
  Serial.print("setup begin\r\n");
  Serial.print(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "init...");
  display.display();
  delay(4000);
  display.clear();
  Serial.print("setup done :)\r\n");

}

    
int timeSinceLastRead = 0;
void loop() {
  // clear the display
  display.clear();
    // Report every 2 seconds.
  if(timeSinceLastRead > 3000) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      display.setFont(ArialMT_Plain_16);
      display.drawString(0, 0, "Failed to read\r\nfrom DHT sensor!");
      display.display();
      
      timeSinceLastRead = 0;
      return;
    }

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.println();
    timeSinceLastRead = 0;
    String tstr = String(t);
    String hstr = String(h);
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 0, "temp");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 20, tstr);
    display.display();
    display.setFont(ArialMT_Plain_16);
    display.drawString(50, 10, "hum");
    display.setFont(ArialMT_Plain_16);
    display.drawString(50, 25, hstr);
    display.display();
    delay(1000);
    display.clear();

  }
  delay(100);
  timeSinceLastRead += 100;
  
}
