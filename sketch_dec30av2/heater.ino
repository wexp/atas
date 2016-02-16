void fire( float target_temp, float temp ) {
  if( temp <= target_temp ) {
    digitalWrite(radPin, HIGH);
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(radPin, LOW);
    digitalWrite(led, LOW);
  }
}

void readTemp() {
  temp_act = tempsens.readTemperature();
  if( temp_act != 998 && temp_act != 999 ) {
      temp = getAvgTemp( temp_act, temp );
  }
}

float getAvgTemp( float in_temp, float avg_temp ) {
  avg_temp -= avg_temp/5;
  avg_temp += in_temp/5;
  return avg_temp;
}
