/*
Automatic Temperature Adjustment System, development version
2015-2016 Erlend Arnesen
Documentation TBD
v0.13 DEV
*/

#include <Time.h>
#include <TimeLib.h>
#include <HTU21D.h>
#include <Wire.h>
#include <RBD_Timer.h>

float temp_norm = 23.0;
float temp_night = 18.0;
float temp_away = 12.0;

struct room
{
  String room_name;
  byte sensor_address;
  byte heater_address;
  float sensor_reading;
  boolean heater_state;
  float target_temperature;
  room* p_next_room;
};

const byte led = 13; 
const byte radPin = 2;
byte mode;
byte nextmode;
float temp = 0;
HTU21D tempsens;
RBD::Timer timer0;
RBD::Timer timer1;
RBD::Timer timer_check;
float temp_set;
boolean timerset = false;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(radPin, OUTPUT);
  Serial.begin(115200);
  tempsens.begin();
  timer0.setTimeout(1000);
  timer0.restart();
  timer_check.setTimeout(30000);
  timer_check.restart();
}

void loop() {
  if(timer_check.isExpired()) {
    timer_check.restart();
    digitalWrite(led, HIGH);
    temp = tempsens.readTemperature();
    digitalWrite(led, LOW);
  }

  if(timer0.onExpired()) {
    mainmenu();
  }

  if(timer1.onExpired()) {
    mode = nextmode;
    timerset = false;
    timer1.stop();
  }
  
  if(Serial.available()) {
    checkSerial();
  }

  switch (mode) {
  case 0: { fire(temp_norm); break; }
  case 1: { fire(temp_night); break; }
  case 2: { fire(temp_away); break; }
  }
}

void fire(float target_temp, float* sensor_reading) {
  if(room->sensor_reading <= room->target_temp) {
    digitalWrite(radPin, HIGH);
  }
  else {
    digitalWrite(radPin, LOW);
  }
}

void checkSerial() {
    char status;
    status = Serial.read();
    switch(status) {
      case 0x31:
        menutemps();
        break;
      case 0x32:
        menutimers();
        break;
      case 0x33:
        menusched();
        break;
      case 0x34:
        menumode();
        break;
      case 0x35:
        menusettime();
        break;
      default:
        break;
    }
}
