// Load Wi-Fi library
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "credentials.h"

const char* ssid = SSID;
const char* password = PASSWORD;

// Red, green, and blue pins for PWM control
const int redPin   = 12;     
const int greenPin = 13;   
const int bluePin  = 14;    

// Setting PWM frequency, channels and bit resolution
const int freq = 5000;
const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;
const int resolution = 8;

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void red() {
  ledcWrite(redChannel,   255);
  ledcWrite(greenChannel, 0);
  ledcWrite(blueChannel,  0);    
}
void green() {
  ledcWrite(redChannel,   0);
  ledcWrite(greenChannel, 255);
  ledcWrite(blueChannel,  0);    
}
void blue() {
  ledcWrite(redChannel,   0);
  ledcWrite(greenChannel, 0);
  ledcWrite(blueChannel,  255);    
}

void setup() {
  Serial.begin(115200);

  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  // Hostname defaults to esp3232-[MAC]
   ArduinoOTA.setHostname("aled");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("aled ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  
  ledcSetup(redChannel,   freq, resolution);
  ledcSetup(greenChannel, freq, resolution);
  ledcSetup(blueChannel,  freq, resolution);
  
  ledcAttachPin(redPin,   redChannel);
  ledcAttachPin(greenPin, greenChannel);
  ledcAttachPin(bluePin,  blueChannel);
  
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  server.on("/rgb", HTTP_POST, [](AsyncWebServerRequest *request){
      String color;
      if (request->hasParam("color", true)) {
          color = request->getParam("color", true)->value();
          Serial.println(color);
          String red,green,blue;
          red   = color.substring(1,3);
          green = color.substring(3,5);
          blue  = color.substring(5,7);
          ledcWrite(redChannel, strtoul(red.c_str(),NULL,16));
          ledcWrite(greenChannel, strtoul(green.c_str(),NULL,16));
          ledcWrite(blueChannel, strtoul(blue.c_str(),NULL,16));    
          Serial.printf("R:%d G:%d B:%d\n",
            strtoul(red.c_str(),NULL,16),
            strtoul(green.c_str(),NULL,16),
            strtoul(blue.c_str(),NULL,16));
      } else {
          color = "Not valid color code";
      }
      request->send(200, "text/plain", "Color is " + color);
    });
  server.onNotFound(notFound);
  server.begin();
  
  for ( int i=0; i<10; i++ ) {
    red();delay(50);
    green();delay(50);
    blue();delay(50);
  }
}

void loop(){
  ArduinoOTA.handle();  
}
