#include <Wire.h>
#include <Time.h>
#include <SparkFunHTU21D.h>
#include <RBD_Timer.h>

float temp_norm = 23.0;
float temp_night = 18.0;
float temp_away = 12.0;
float temp_act = 0.0;
float temp = 0.0;

const byte led = 13; 
const byte radPin = 2;
byte mode;
byte nextmode;

HTU21D tempsens;
RBD::Timer timer0;
RBD::Timer timer1;
RBD::Timer timer_check;
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
  temp_act = tempsens.readTemperature();
  temp = temp_act;
  readTemp();
}

void loop() {
  if(timer_check.isExpired()) {
    timer_check.restart();
    readTemp();
  }
  
  if(timer0.onExpired()) {
    timer0.restart();
    if( timer_check.isExpired() ) {
      ansiClear();
    }
    mainmenu();
  }
  if(timer1.onExpired()) {
    mode = nextmode;
    timerset = false;
    ansiClear();
  }
  
  getSerial();

  switch( mode ) {
    case 0:
      fire( temp_norm, temp );
      break;
    case 1:
      fire( temp_night, temp );
      break;
    case 2:
      fire( temp_night, temp );
      break;
  }
}
