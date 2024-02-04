//#include "M5Stack.h"
#include <M5AtomS3.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "creds.h"

const char* ssid = LOCAL_SSID;
const char* password = LOCAL_PWD;
const long utcOffsetInSeconds = 11 * 3600;  // UTC offset for Sydney (11 hours)

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

uint16_t time_pos_x, time_pos_y;
int previousHour, previousMinute;

void setup() {
  M5.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Initialize the NTPClient
  timeClient.begin();

  time_pos_x = M5.Lcd.width() / 2 - 45;
  time_pos_y = M5.Lcd.height() / 2 - 10;
  M5.Lcd.fillScreen(TFT_YELLOW);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(TFT_BLACK);
}

void loop() {
  // Update the time
  timeClient.update();

  // Get the current hour
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();

  // Check if it's nighttime (between 10 pm and 6 am)
  if (currentHour >= 22 || currentHour < 6) {
    M5.Lcd.setBrightness(10); // Set dimmed brightness
  } else {
    M5.Lcd.setBrightness(50); // Set normal brightness (255 is maximum)
  }

  if (previousHour != currentHour ||
      previousMinute != currentMinute) {
      // TODO: Use a framebuffer and only update the changed digit(s)
      // Print the current time
      M5.Lcd.fillScreen(TFT_YELLOW);
      M5.Lcd.setCursor(time_pos_x, time_pos_y);
      M5.Lcd.setRotation(2);
      M5.Lcd.printf("%02d:%02d", currentHour, currentMinute);
  }
  
  previousHour = currentHour;
  previousMinute = currentMinute;

  delay(30000);
}
