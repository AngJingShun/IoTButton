#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

#include <ESP8266HTTPClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <FirebaseArduino.h>     //https://github.com/firebase/firebase-arduino

#define FIREBASE_HOST "rebutton-47809.firebaseio.com"            //Link to Rebutton Firebase Host
#define FIREBASE_AUTH "sjzdPhXHU1ePZv3vIPKbHBbjjAFQStSnAyjIR0sr" //Rebutton Firebase Database Secrets

#define DEBUG 0 // Change this number to 1 during debugging stage

void setup() {
    // Setup code here, to run once:
       
    if (DEBUG == 1) {
    Serial.begin(115200);
    }
  
    WiFiManager wifiManager;   //WiFiManager local intialisation of the library.
    
    //reset saved Wi-Fi settings on ESP8266
    //wifiManager.resetSettings();
    
    //To set up custom ip for the configuration portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    /* Not necessary since the wifiManager.autoConnect() checks for connection
       and print to serial monitor if DEBUG = 1
     if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Test1");
      if(DEBUG == 1){
        Serial.println("Connect to Rebutton to configure Wi-Fi network");
      }
    } */

    //Programme fetches ssid and pass from eeprom and tries to connect
    //if unable to connect, an access point with the specified name will be set up
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("Rebutton", "internet");

    if (WiFi.status() == WL_CONNECTED) {
        if (DEBUG == 1) {
          Serial.println("Succesfully Connected to Wi-Fi!"); //If you get here, you are successfully connected to the internet.
          Serial.println(WiFi.localIP());
        }
      }

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);    
}

 void loop() {
  Firebase.setInt("devices/device3/state", 2);
  
  if ( Firebase.failed() ) {
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
    return;
  }

  //Serial.println("Void Loop Started");

  if (WiFi.status() == WL_CONNECTED) {            //Check WiFi connection status

    HTTPClient http;                              //Declare an object of class HTTPClient

    http.begin("http://maker.ifttt.com/trigger/ESP8266/with/key/dby73iqPkl150Bzp36EY7y");  //Specify request destination
    int httpCode = http.GET();                                                             //Send the request

    if (httpCode > 0) {                    //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);             //Print the response payload
    } else {
      //Serial.println("Error in GET Request");
      return;
    }
    http.end();   //Close connection
  }
  ESP.deepSleep(0);    //Put the device to deep sleep to save battery
 }
