/*
This sketch brings together a number of sensors to inform the volume and in flow status of a water tank
This information is then conveyed locally via RAG LEDs and communicated wider via MQTT broker
*/

#include <Arduino_ConnectionHandler.h>  //connection handler library is used to check WiFi connection and reconnect
#include "secrets.h"                    //loads secrets.h file (note "" v <>)
#include <ArduinoMqttClient.h>          //MQTT library

WiFiConnectionHandler conMan(SECRET_SSID, SECRET_PASS);  //Arduino_ConnectionHandler.h declaring the login info
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

//variables
unsigned long now = 0;          //global - now is used for multiple timers
unsigned long volumeTimer = 0;  //volume - initiates the timer for the volume polling
unsigned long flowTimer = 0;    //flow - initiates the timer for the volume polling
unsigned long flowTime = 0;     //flow - calculated time over which the flowcounts are poled
int level = 0;                  //volume - for the non contact to report 1/0
volatile int count = 0;         //flow - counts the intrrupt events from the hall effect flow meter
int flowcount = 0;              //flow - takes the counts while the interrupt is disabled
float duration = 0;             //volume - TOF of ultrasound used to calculate "distance"
float distance = 0;             //volume - distance calculated from TOF used to calculate "Volume"
float Volume = 0;               //volume - "Volume" is returned from the "volume()" function
float Flow = 0;                 //flow - "Flow" is returned from the "flow()" function
long unsigned testTimer = 0;    //LEDtest - timer
byte blink = 0;                 //LEDblink - blink 1 / 0
long unsigned ledTimer = 0;     //LEDblink - timer
long unsigned mqttTimer = 0;    //comms - timer

//constants
const unsigned long volumePeriod = 10000;  //volume - interval between volume polling (ms)
const unsigned long flowPeriod = 10000;    //volume -interval between flow polling (ms)
const long unsigned testPeriod = 20000;    //LEDtest - period between LED tests
const long unsigned startPeriod = 10000;   //LEDstart - light LEDs while initial readings are taken
const long unsigned ledPeriod = 1000;      //LEDblink - on / off
const long unsigned mqttPeriod = 10000;    //comms - MQTT update cadence
const float volumeThreshold = 2.6;         //LED - red / amber threshold for ultrasound value
const float flowThreshold = 10;            //LED - red / amber threshold for ultrasound value
const char broker[] = "192.168.0.50";      //setup - broker IP address
int port = 1883;                           //setup - port to broker
const char topicFlow[] = "Flow";           //comms - publishes to topic "tank/flow"
const char topicVolume[] = "Volume";       //comms - publishes to topic "tank\level"


//hardware
int levelPin = 3;   //Sets pin 3 to be 'int & levelPin'
int flowPin = 2;    //Sets pin 2 to be 'int & flowPin'  (which is an interrupt pin)
int trigPin = 8;    //sets pin 8 to be 'int & trigPin'
int echoPin = 9;    //sets pin 8 to be 'int & echoPin'
int ledIndicators;  //State machine 'sort of hardrware'
int ledGreen = 4;   //sets pin 4 to be green LED
int ledAmber = 5;   //sets pin 5 to be yellow LDE
int ledRed = 6;     //sets pin 6 to be red LDE


enum ledIndicators { test,
                     green,
                     amber,
                     red,
                     greenBlink,
                     amberBlink,
                     redBlink };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  //serial connection
  while (!Serial) {
  }

  setDebugMessageLevel(DBG_INFO);  //conMan - one of 5 levels of debug messages

  conMan.addCallback(NetworkConnectionEvent::CONNECTED, onNetworkConnect);        //conMan = callback notified of network connection events
  conMan.addCallback(NetworkConnectionEvent::DISCONNECTED, onNetworkDisconnect);  //conMan = callback notified of network connection events
  conMan.addCallback(NetworkConnectionEvent::ERROR, onNetworkError);              //conMan = callback notified of network connection events

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT_PULLUP);
  pinMode(flowPin, INPUT);
  pinMode(ledGreen, OUTPUT);                                     //sets ledgreen  (pin 4) to output
  pinMode(ledAmber, OUTPUT);                                     //sets ledamber (pin 5) to output
  pinMode(ledRed, OUTPUT);                                       //sets ledred (pin 6) to output                                   //sets flowPin to input
  attachInterrupt(digitalPinToInterrupt(2), FlowCount, RISING);  //configures interrupt 0 (apparently pin 2 on nano / uno?) to run function 'FlowCount' on a rising pulse
}

void loop() {
  // put your main code here, to run repeatedly:
  volume();
  flow();
  LEDs();
  Comms();
}
