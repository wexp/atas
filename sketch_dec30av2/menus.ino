void mainmenu() {
    ansiHome();
    Serial.println(F("ATAS running. Press a number to change operating mode."));
    Serial.print(F("Time: "));
    Serial.print(hour());
    Serial.print(F(":"));
    Serial.print(minute());
    Serial.print(F("  "));
    Serial.print(day());
    Serial.print(F("."));
    Serial.print(month());
    Serial.print(F("."));
    Serial.println(year());
    Serial.println(F("1.Temperatures  2.Timers  3. N/A  4.Modes  5.Set time"));
    Serial.println();
    if(timerset) {
      Serial.print(F("Timer activated, next mode will be: "));
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
      Serial.print(F("Time left: "));
      Serial.println(minutes_left);
    }
    Serial.print(F("Current temperature: "));
    Serial.println(temp);
    Serial.print(F("Target temperature: "));
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
    if(mode == 0) {
      Serial.println(F("Normal"));
    }
    if(mode == 1) {
      Serial.println(F("Night"));
    }
    if(mode == 2) {
      Serial.println(F("Away"));
    }
}

void menutemps() {
  boolean done = false;
  char choice;
  while (!done) { 
    long temptemp = 0;
    ansiClear();
    ansiHome();
    Serial.print(F("1. Normal temperature: "));
    Serial.println(temp_norm);
    Serial.print(F("2. Night temperature: "));
    Serial.println(temp_night);
    Serial.print(F("3. Away temperature: "));
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
      case 'x':
        done = true;
        ansiClear();
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
        ansiClear();
        afterdone = true;
        break;
      case 0x32:
        nextmode = 1;
        ansiClear();
        afterdone = true;
        break;
      case 0x33:
        nextmode = 2;
        ansiClear();
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
      ansiClear();
      done = true;
      break;
    case 0x32:
      mode = 1;
      ansiClear();
      done = true;
      break;
    case 0x33:
      mode = 2;
      ansiClear();
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
  ansiHome();
  Serial.println(F("Enter UNIX time:"));
  setTime(getLong());
  ansiClear();
}
