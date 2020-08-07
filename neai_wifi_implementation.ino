#include <SPI.h>
#include <WiFiNINA.h>
#include <Adafruit_SleepyDog.h>
#include <Pushsafer.h>

#include "arduino_secrets.h"
#include <NanoEdgeAI.h>
#define SAMPLES 128
#define NUMBER_LEARN 100

int sensorPin = A0;    // select the input pin for the potentiometer
int nbr_sleep_cycles = 4; //Number of sleepcycles between data reads(1 cycle = 15seconds)
int nbr_sleep_cycles_alert = 120; //Number of sleepcycles after an alert (1 cycle = 15seconds)
float sensor_buffer[SAMPLES] = {0};

float sim1 = 100.0;
float sim2 = 100.0;
float avg = 0.0;
 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiClient client;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  //Comment out when using without serial readouts
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  //}

  Serial.println("Start Learning");
  NanoEdgeAI_initialize();
  for (int i = 0; i < NUMBER_LEARN; i++) {
    get_current_data();
    NanoEdgeAI_learn(sensor_buffer);
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
    digitalWrite(LED_BUILTIN, HIGH);   
  }
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Learning Done!");
}

void loop() {
  get_current_data();
  float similarity = NanoEdgeAI_detect(sensor_buffer);
  avg = (similarity + sim1 + sim2) / 3.0;
  sim2 = sim1;
  sim1 = similarity;
  
  if (avg <= 90.0){
    while (status != WL_CONNECTED) {
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.println(ssid);
      // Connect to WPA/WPA2 network:
      status = WiFi.begin(ssid, pass);

      // wait 5 seconds for connection:
      delay(5000);
    }
    // you're connected now, so print out the data:
    Serial.print("You're connected to the network");
    digitalWrite(LED_BUILTIN, HIGH);   
    delay(500);                  
    digitalWrite(LED_BUILTIN, LOW);
    printCurrentNet();
    printWifiData();

    Pushsafer pushsafer(PushsaferKey, client);
    
    struct PushSaferInput input;
    input.message = "The bag needs to be changed!";
    input.title = "Hello!";
    input.vibration = "1";
    input.icon = "1";
    input.iconcolor = "#FFCCCC";
    input.priority = "1";
    input.device = "a";
    Serial.println(avg);
    Serial.println(pushsafer.sendEvent(input));
    Serial.println("Alarm sent");
    sim1 = 100.0;
    sim2 = 100.0;
    delay(5000);//Give time to transmit before shutting wifi
    WiFi.end();
    delay(10000);
      for (int i =0; i < nbr_sleep_cycles_alert; i++){
        Watchdog.sleep(15000);
      }
  }
//  for (int i =0; i < nbr_sleep_cycles; i++){
//    Watchdog.sleep(15000);
//  }
  delay(200);
}

void get_current_data() {
  for (uint16_t i = 0; i < SAMPLES; i++) {
    sensor_buffer[i] = analogRead(sensorPin);
  }
}

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  printMacAddress(mac);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  printMacAddress(bssid);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void printMacAddress(byte mac[]) {
  for (int i = 5; i >= 0; i--) {
    if (mac[i] < 16) {
      Serial.print("0");
    }
    Serial.print(mac[i], HEX);
    if (i > 0) {
      Serial.print(":");
    }
  }
  Serial.println();
}
