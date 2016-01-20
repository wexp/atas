void menutemps() {
  boolean done = false;
  char choice;
  while (!done) { 
    long temptemp = 0;
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
  boolean timeoutdone = false;
  boolean afterdone = false;
  char choice;
  while (!timeoutdone) {
    unsigned long _timerset;
    ansiClear();
    ansiHome();
    Serial.println(F("Set timeout in hours:"));
    _timerset = getLong();
    if(_timerset >= 0 || _timerset <= 86400000) {
      timer1.setTimeout(_timerset * 3600000);
      timeoutdone = true;
      timer1.restart();
    }
    else {
    Serial.println(F("Error, must be between 0 and 24 hours."));
    }
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
    if(!timer1.isExpired() || !timer1.isStopped()) {
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
    if(choice == 0 || choice == 1 || choice == 2) {
      mode = choice;
    }
/*
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
*/

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

void mainmenu() {
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
    switch (nextmode) {
      case 0: { Serial.println(F("Normal")); break; }
      case 1: { Serial.println(F("Night")); break; }
      case 2: { Serial.println(F("Away")); break; }
    }
    Serial.print(F("Time left:"));
    ansiTab();
    ansiTab();
    ansiTab();
    ansiTab();
    Serial.println(timer1.getInverseValue() / 60000);
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
  switch (mode) {
    case 0: { Serial.println(temp_norm); break; }
    case 1: { Serial.println(temp_night); break; }
    case 2: { Serial.println(temp_away); break; }
  }
  Serial.print(F("Current mode: "));
  ansiTab();
  ansiTab();
  ansiTab();
  ansiTab();
  switch (mode) {
    case 0: { Serial.println(F("Normal")); break; }
    case 1: { Serial.println(F("Night")); break; }
    case 2: { Serial.println(F("Away")); break; }
  }
}
