#include <Time.h>
#include <TimeLib.h>

#include <HTU21D.h>
#include <Wire.h>
#include <RBD_Timer.h>

float temp_norm = 23.0;
float temp_night = 18.0;
float temp_away = 12.0;

const byte led = 13; 
/*
const byte SSlatchPin = 10;
const byte SSclockPin = 9;
const byte SSdataPin = 8;
*/
const byte radPin = 2;
byte mode;
byte nextmode;
float temp = 0;
HTU21D tempsens;
RBD::Timer timer0;
RBD::Timer timer1;
RBD::Timer timer_check;
const int numbers[] = {B10000001,B11110011,B01001001,B01100001,B00110011,B00100101,B00000101,B11110001,B00000001,B00110001};
float temp_set;
char status;
boolean timerset = false;

void setup() {
//  pinMode(SSlatchPin, OUTPUT);
//  pinMode(SSclockPin, OUTPUT);
//  pinMode(SSdataPin, OUTPUT);
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
    timer0.restart();
    ansiHome();
    Serial.println(F("ATAS running. Press a number to change operating mode."));
    Serial.print(F("Time: "));
    Serial.print(hour());
    Serial.print(F(":"));
    Serial.print(minute());
    ansiTab();
    Serial.print(day());
    Serial.print(F("."));
    Serial.print(month());
    Serial.print(F("."));
    Serial.println(year());
    Serial.println(F("1.Temperatures  2.Timers  3. Schedules (N/A)  4.Modes  5.Set time"));
    Serial.println();
    if(timerset) {
      Serial.print(F("Timer activated, next mode will be: "));
      ansiTab();
      if(nextmode == 0) {
        Serial.println(F("Normal"));
      }
      if(nextmode == 1) {
        Serial.println(F("Night"));
      }
      if(nextmode == 2) {
        Serial.println(F("Away"));
      }
      unsigned int minutes_left;
      minutes_left = timer1.getInverseValue() / 60000;
      Serial.print(F("Time left:"));
      ansiTab();
      ansiTab();
      ansiTab();
      ansiTab();
      Serial.println(minutes_left);
    }
    Serial.print(F("Current temperature: "));
    ansiTab();
    ansiTab();
    ansiTab();
    Serial.println(temp);
    Serial.print(F("Target temperature:"));
    ansiTab();
    ansiTab();
    ansiTab();
    if(mode == 0) {
      Serial.println(temp_norm);
    }
    if(mode == 1) {
      Serial.println(temp_night);
    }
    if(mode == 2) {
      Serial.println(temp_away);
    }
    Serial.print(F("Current mode: "));
    ansiTab();
    ansiTab();
    ansiTab();
    ansiTab();
    if(mode == 0) {
      Serial.println(F("Normal"));
    }
    if(mode == 1) {
      Serial.println(F("Night"));
    }
    if(mode == 2) {
      Serial.println(F("Away"));
    }
    if(radPin == HIGH) {
      Serial.println(F("HEATING"));
    }
    else if(radPin == LOW) {
      Serial.println(F("NOT HEATING"));
    }
  }

  if(timer1.onExpired()) {
    mode = nextmode;
  }
  
  while (Serial.available()) {
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

  if(mode == 0) {
    if(temp <= temp_norm) {
      digitalWrite(radPin, HIGH);
    }
    else {
      digitalWrite(radPin, LOW);
    }
  }
  if(mode == 1) {
    if(temp <= temp_night) {
      digitalWrite(radPin, HIGH);
    }
    else {
      digitalWrite(radPin, LOW);
    }
  }
  if(mode == 2) {
    if(temp <= temp_away) {
      digitalWrite(radPin, HIGH);
    }
    else {
      digitalWrite(radPin, LOW);
    }
  }
}

void menutemps() {
  boolean done = false;
  char choice;
  while (!done) { 
    long temptemp = 0;
    status = 0;
    ansiClear();
    ansiHome();
    Serial.print(F("1. Normal temperature:"));
    ansiTab();
    Serial.println(temp_norm);
    Serial.print(F("2. Night temperature:"));
    ansiTab();
    Serial.println(temp_night);
    Serial.print(F("3. Away temperature:"));
    ansiTab();
    Serial.println(temp_away);
    Serial.println();
    Serial.println(F("Press a number to change, or press x to exit."));
    choice = 0;
    choice = getSingleChar() - 0;
    switch (choice) {
      default:
        choice = 0;
        break;
      case 0x31:
        Serial.println(F("Input your new normal temperature."));
        temptemp = getLong();
        if (temptemp >= 0.0 || temptemp <= 30.0) 
        {
          temp_norm = temptemp;
          ansiClear();
          done = true;
          break;
        }
        else
        {
        Serial.println(F("Error, must be between 0.0 and 30.0."));
        }
      case 0x32:
        Serial.println(F("Input your new night temperature."));
        temptemp = getLong();
        if (temptemp >= 0.0 || temptemp <= 30.0) 
        {
          temp_night = temptemp;
          ansiClear();
          done = true;
          break;
        }
        else
        {
        Serial.println(F("Error, must be between 0.0 and 30.0."));
        }
      case 0x33:
        Serial.println(F("Input your new away temperature."));
        temptemp = getLong();
        if (temptemp >= 0.0 || temptemp <= 30.0) 
        {
          temp_away = temptemp;
          ansiClear();
          done = true;
          break;
        }
        else
        {
        Serial.println(F("Error, must be between 0.0 and 30.0."));
        }  
      case 0x78:
        done = true;
        break;
      }
  }
}

void menutimers() {
//  boolean nowdone = false;
  boolean timeoutdone = false;
  boolean afterdone = false;
  char choice;
/*  while (!nowdone) {
    ansiClear();
    ansiHome();
    Serial.println(F("To set a timer, select desired current mode."));
    Serial.println(F("1 is normal, 2 is night, 3 is away."));
    Serial.println(F("Otherwise press x to exit, d to clear all timers."));
    choice = 0;
    choice = getSingleChar() - 0;
    switch (choice) {
      default:
        choice = 0;
        break;
      case 0x31:
        mode = 0;
        nowdone = true;
        break;
      case 0x32:
        mode = 1;
        nowdone = true;
        break;
      case 0x33:
        mode = 2;
        nowdone = true;
        break;
      case 0x78:
        nowdone = true;
        timeoutdone = true;
        afterdone = true;
        break;
      case 0x64:
        timer0.setTimeout(1);
        nowdone = true;
        timeoutdone = true;
        afterdone = true;
        timerset = false;
        break;
    }
  }*/
  while (!timeoutdone) {
    unsigned long _timerset;
    ansiClear();
    ansiHome();
    Serial.println(F("Set timeout in hours:"));
    _timerset = getLong();
    timer1.setTimeout(_timerset * 3600000);
    timeoutdone = true;
    timer1.restart();
  }
  while (!afterdone) {
    ansiClear();
    ansiHome();
    Serial.println(F("Select desired mode to switch to."));
    Serial.println(F("1 is normal, 2 is night, 3 is away."));
    choice = 0;
    choice = getSingleChar() - 0;
    switch (choice) {
      default:
        choice = 0;
        break;
      case 0x31:
        nextmode = 0;
        afterdone = true;
        break;
      case 0x32:
        nextmode = 1;
        afterdone = true;
        break;
      case 0x33:
        nextmode = 2;
        afterdone = true;
        break;
    }
    if(!timer1.isExpired()) {
      timerset = true;
     }
  }
  ansiClear();
}

void menumode() {
  boolean done = false;
  char choice;
  while (!done) {
    ansiClear();
    Serial.println(F("Select desired mode to switch to."));
    Serial.println(F("1 is normal, 2 is night, 3 is away."));
    choice = 0;
    choice = getSingleChar() - 0;
    switch (choice) {
      default:
      choice = 0;
      break;
    case 0x31:
      mode = 0;
      done = true;
      break;
    case 0x32:
      mode = 1;
      done = true;
      break;
    case 0x33:
      mode = 2;
      done = true;
      break;
    }
  }
  ansiClear();
}

void menusched() {
  
}

void menusettime() {
  ansiClear();
  Serial.println(F("Enter UNIX time:"));
  setTime(getLong());
  ansiClear();
}

/*void SSDisplayActualTemp() {
  int inttemp = (int) temp;

  if(inttemp >= 30) {
    inttemp = inttemp - 30; }
  else if(inttemp >= 20) {
    inttemp = inttemp - 20; }
  else if(inttemp >= 10) {
    inttemp = inttemp - 10; }

  digitalWrite(SSlatchPin, LOW);
  shiftOut(SSdataPin, SSclockPin, MSBFIRST, numbers[inttemp]);
  digitalWrite(SSlatchPin, HIGH);
}*/

void ansiClear()
// send ANSI clear screen command
// used by ANSI output format
{
  // send ESC [2J
  // which will clear the screen on an ANSI terminal
  Serial.write(27);
  Serial.write(91);
  Serial.write(50);
  Serial.write(74);
}

void ansiHome()
// ANSI terminal command to move the cursor to 0,0 without clearing screen
// used by ANSI output format
{
  // send ESC [H 
  // which puts the cursor in the home 0,0 position on an ANSI terminal
  Serial.write(27);
  Serial.write(91);
  Serial.write(72);
}

void ansiTab()
// send ANSI "tab" character
// used by ANSI output format
{
  // send tab char
  Serial.write(9);
}

long getLong()
// wait for a character to be input (end with return), allows backspace
{
  char mystring[10];
  char mychar;
  int x = 0;
  boolean done = false;
  
  // input a string of characters from the user
  
  while (!done)
  {
    mychar = getChar();
    if ((mychar == 0x0D) || (mychar == 0x0A)) // carriage return or line feed?
    {
      // if this is the first character in the string, ignore it
      if (x > 0)
      {
        // otherwise, terminate the string with 0x00 and exit
        mystring[x] = 0;
        done = true;
      }
    }
    else
    {
      if ((mychar == 0x08) && (x > 0)) // backspace?
      {
        // simulate a backspace - back up, print a space to erase character, and backspace again
        Serial.write(0x08);
        Serial.print(F(" "));
        Serial.write(0x08);
        x--;
      }
      else // a valid character?
      {
        // add it to the string
        if (x < 10)
        {
          Serial.print(mychar);
          mystring[x] = mychar;
          x++;
        }
      }
    }
  }
  // convert string to long value and return
  return(atol(mystring));
}

char getChar()
// wait for one character to be typed (and convert to uppercase if it's alphabetic)
{
  while (!Serial.available())
    {;} // wait here forever for a character
  return(toupper(Serial.read())); // return the upper case character
}

char getSingleChar()
{
  char mychar;
  
  // read serial characters until one isn't carriage return or newline
  do
    mychar = getChar();
  while ((mychar == 0x0D) || (mychar == 0x0A));
  return(mychar);
}