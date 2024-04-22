// TCS230 or TCS3200 pins wiring to Arduino
#define S0 30
#define S1 31
#define S2 32
#define S3 33
#define sensorOut 40


void setupColour() {

  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);

  // Setting frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
}

int avgFreq()  //find average frequency for color sensor (avg of 5 readings)
{
  int reading;
  int total = 0;

  for (int i = 0; i < 5; i += 1) {
    reading = pulseIn(sensorOut, LOW);
    total += reading;
    delay(20);
  }
  return total / 5;
}

int findColour()  //find the color of the paper
{
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  // Reading the output frequency for red
  redFrequency = avgFreq();//pulseIn(sensorOut, LOW);
  redColour = map(redFrequency, 130, 860, 255, 0);
  delay(100);

  // Setting GREEN (G) filtered photodiodes to be read -------------------------
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  delay(100);

  // Reading the output frequency for green
  greenFrequency = avgFreq(); // pulseIn(sensorOut, LOW);
  greenColour = map(greenFrequency, 260, 1970, 255, 0);
  delay(100);

  // Setting BLUE (B) filtered photodiodes to be read ------------------------
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  delay(100);

  // Reading the output frequency for blue
  blueFrequency = avgFreq();//pulseIn(sensorOut, LOW);
  blueColour = map(blueFrequency, 325, 950, 255, 0);
  delay(100);

  int colourCode;
  /*
    if (redColour > 280 && greenColour > 280 && blueColour > 280) {
      colourCode = 0;  //0 represents white
    } else if (redColour > greenColour && redColour > blueColour) {

      colourCode = 1;  //1 represents red
      red();

    } else if (greenColour > redColour && greenColour > blueColour) {

      colourCode = 2; //2 represents green
      green();

    } else {
      colourCode = 0;
    }
  */
  if (redFrequency + greenFrequency + blueFrequency < 2000) {
    colourCode = 0; //white
  } else if (abs(redFrequency - greenFrequency) < 200) {
    colourCode = 2; //green
    green();
  } else {
    colourCode = 1; //red
    red();
  }
  return colourCode;
}
