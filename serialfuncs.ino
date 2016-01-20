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
