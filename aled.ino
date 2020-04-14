// Load Wi-Fi library
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include "credentials.h"

AsyncWebServer server(80);

const char* ssid = SSID;
const char* password = PASSWORD;

// Decode HTTP GET value
String redString = "0";
String greenString = "0";
String blueString = "0";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;

// Variable to store the HTTP req  uest
String header;

// Red, green, and blue pins for PWM control
const int redPin = 14;     
const int greenPin = 13;   
const int bluePin = 12;    

// Setting PWM frequency, channels and bit resolution
const int freq = 5000;
const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;

const int resolution = 8;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
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
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
   ArduinoOTA.setHostname("led");

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

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  
  // configure LED PWM functionalitites
  ledcSetup(redChannel, freq, resolution);
  ledcSetup(greenChannel, freq, resolution);
  ledcSetup(blueChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(redPin, redChannel);
  ledcAttachPin(greenPin, greenChannel);
  ledcAttachPin(bluePin, blueChannel);
  
//  // Connect to Wi-Fi network with SSID and password
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  // Print local IP address and start web server
//  Serial.println("");
//  Serial.println("WiFi connected.");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
//  server.begin();
    server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
    // Send a POST request to <IP>/post with a form field message set to <message>
    server.on("/rgb", HTTP_POST, [](AsyncWebServerRequest *request){
        String color;
        if (request->hasParam("color", true)) {
            color = request->getParam("color", true)->value();
            Serial.println(color);
            String red,green,blue;
            red = color.substring(1,3);
            green = color.substring(3,5);
            blue = color.substring(5,7);
            ledcWrite(redChannel, strtoul(red.c_str(),NULL,16));
            ledcWrite(greenChannel, strtoul(green.c_str(),NULL,16));
            ledcWrite(blueChannel, strtoul(blue.c_str(),NULL,16));    
              Serial.printf("R:%d G:%d B:%d\n",strtoul(red.c_str(),NULL,16),
              strtoul(green.c_str(),NULL,16),
              strtoul(blue.c_str(),NULL,16));
        } else {
            color = "No message sent";
        }
        request->send(200, "text/plain", "Hello, POST: " + color);
    });
    server.onNotFound(notFound);
    server.begin();
}
void red() {
  ledcWrite(redChannel, 255);
  ledcWrite(greenChannel, 0);
  ledcWrite(blueChannel, 0);    
}
void green() {
  ledcWrite(redChannel, 0);
  ledcWrite(greenChannel, 255);
  ledcWrite(blueChannel, 0);    
}
void blue() {
  ledcWrite(redChannel, 0);
  ledcWrite(greenChannel, 0);
  ledcWrite(blueChannel, 255);    
}
void white8() {
  const int resolution = 8;  
  int max = 255;
  ledcSetup(redChannel, freq, resolution);
  ledcSetup(greenChannel, freq, resolution);
  ledcSetup(blueChannel, freq, resolution);
  
  ledcWrite(redChannel, max);
  ledcWrite(greenChannel, max);
  ledcWrite(blueChannel, max);    
}
void white10() {
  const int resolution = 10;
  int max = 2048;
  ledcSetup(redChannel, freq, resolution);
  ledcSetup(greenChannel, freq, resolution);
  ledcSetup(blueChannel, freq, resolution);
  
  ledcWrite(redChannel, max);
  ledcWrite(greenChannel, max);
  ledcWrite(blueChannel, max);    
}
void loop(){
  ArduinoOTA.handle();  
  int r,g,b;
//  Serial.println("Red");red();delay(1000);
//  Serial.println("Green");green();delay(1000);
//  Serial.println("Blue");blue();delay(1000);
//  Serial.println("White8");white8();delay(1000);
//  Serial.println("White10");white10();delay(1000);
  
//  for ( r=0;r<255;r++ ) {
//    Serial.println(r);
//    ledcWrite(redChannel, r);
//    ledcWrite(greenChannel, r);
//    ledcWrite(blueChannel, r);    
//    delay(100);
//  }

}
