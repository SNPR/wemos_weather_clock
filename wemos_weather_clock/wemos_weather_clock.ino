#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN D5
#define DATA_PIN D7
#define CS_PIN D8

#define SPEED_TIME 150
#define PAUSE_TIME 0

#define WI_FI_STATUS_UPDATE_INTERVAL 500
#define UTC_OFFSET_IN_SECONDS 18000 // provide your own offset here
#define TIME_DISPLAY_INTERVAL 10000
#define DATE_DISPLAY_INTERVAL 3000
#define DOTS_SHOW_INTERVAL 1000
#define WEATHER_UPDATE_INTERVAL 600000
#define WEATHER_API_URL "http://api.weatherapi.com/v1/current.json?key={provide your own api key and city here}"


MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

const char* ssid = "your wi-fi SSID";
const char* password = "your wi-fi password";

char timeArray[9];
char dateArray[6];
char weatherArray[9];

unsigned long lastSwitchTime = 0;
unsigned long switchInterval = TIME_DISPLAY_INTERVAL;
unsigned long lastWeatherUpdateTime = 0;
unsigned long lastDotsShowTime = 0;

bool dotsShown = true;

enum DisplayState {
  DisplayTime,
  DisplayDate,
  DisplayWeather
};

DisplayState displayState = DisplayTime;

WiFiUDP ntpUDP;
WiFiClient wifiClient;
NTPClient timeClient(ntpUDP, "pool.ntp.org", UTC_OFFSET_IN_SECONDS);

void setup() {
  Serial.begin(115200);
  P.begin();
  P.setIntensity(1);
  P.displayText("WI-FI wait", PA_LEFT, SPEED_TIME, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  P.displayAnimate();

  WiFi.begin(ssid, password);

  unsigned long lastConnectionAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED) {
    P.displayAnimate();
    if (millis() - lastConnectionAttemptTime >= WI_FI_STATUS_UPDATE_INTERVAL) {
      Serial.println("Connecting to WiFi...");
      lastConnectionAttemptTime = millis();
    }
    yield();
  }

  Serial.println("Connected to WiFi.");
  timeClient.begin();
  updateWeather();
  lastWeatherUpdateTime = millis();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
  }


  if (millis() - lastWeatherUpdateTime >= WEATHER_UPDATE_INTERVAL) {
    updateWeather();
    lastWeatherUpdateTime = millis();
  }

  if (millis() - lastSwitchTime > switchInterval) {
    switch (displayState) {
      case DisplayTime:
        displayState = DisplayDate;
        switchInterval = TIME_DISPLAY_INTERVAL;
        updateTime();
        break;
      case DisplayDate:
        displayState = DisplayWeather;
        switchInterval = DATE_DISPLAY_INTERVAL;
        displayDate();
        break;
      case DisplayWeather:
        displayState = DisplayTime;
        switchInterval = DATE_DISPLAY_INTERVAL;
        displayWeather();
        break;
    }

    lastSwitchTime = millis();
  }



  if (displayState == DisplayDate) {
    if (millis() - lastDotsShowTime > DOTS_SHOW_INTERVAL) {
      lastDotsShowTime = millis();
      dotsShown = !dotsShown;
      updateTime();
    };

    String newTime = formateTime(timeClient.getHours(), timeClient.getMinutes());
    if (newTime != String(timeArray)) {
      P.displayReset();
      updateTime();
    }
  }


  P.displayAnimate();
}


void updateWeather() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(wifiClient, WEATHER_API_URL);
    int httpCode = http.GET();
    Serial.println("httpCode");
    Serial.println(httpCode);

    if (httpCode > 0) {
      String payload = http.getString();
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);
      float temperature = doc["current"]["temp_c"];
      Serial.print("Temperature in Yekaterinburg: ");
      Serial.println(temperature);

      String temperatureString = String(temperature, 0) + "\xB0" + "C";
      temperatureString.toCharArray(weatherArray, sizeof(weatherArray));
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
}


void displayWeather() {
  if (P.displayAnimate()) {
    P.displayReset();
    P.displayText(weatherArray, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);
  }
}


void displayDate() {
  if (WiFi.status() == WL_CONNECTED) {

    String date = timeClient.getFormattedDate();
    String day = date.substring(8, 10);
    String month = date.substring(5, 7);

    String formattedDate = day + "." + month;

    formattedDate.toCharArray(dateArray, sizeof(dateArray));
    if (P.displayAnimate()) {
      P.displayReset();
      P.displayText(dateArray, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);
    }
    // Serial.println(dateArray);
  }
}

String formateTime(int hours, int minutes) {
  String strHours = String(hours);
  String strMinutes = minutes < 10 ? "0" + String(minutes) : String(minutes);
  String middlePart = dotsShown ? " : " : "   ";

  return strHours + middlePart + strMinutes;
}


void updateTime() {
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();

    // Serial.println(timeClient.getFormattedDate());

    String time = formateTime(timeClient.getHours(), timeClient.getMinutes());

    time.toCharArray(timeArray, sizeof(timeArray));
    if (P.displayAnimate()) {
      P.displayReset();
      P.displayText(timeArray, PA_CENTER, 0, 0, PA_NO_EFFECT, PA_NO_EFFECT);
    }
    // Serial.println(timeArray);
  }
}

