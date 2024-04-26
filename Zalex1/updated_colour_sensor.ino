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

int findShortestDist()
{
  int redDist;
  int greenDist;
  int blueDist;

  redDist = 255 - redColour;
  redDist = redDist*redDist;
  greenDist = 255 - greenColour;
  greenDist = greenDist*greenDist;
  blueDist = 255 - blueColour;
  blueDist = blueDist*blueDist;

  redColour = redColour*redColour;
  greenColour = greenColour*greenColour;
  blueColour = blueColour * blueColour;

  int colourDist[3]; //0 for red distance, 1 for green distance, 2 for white distance
  colourDist[0] = redDist + greenColour + blueColour;
  colourDist[1] = redColour + greenDist + blueColour;
  colourDist[2] = redDist + greenDist + blueDist;

  int minDist = 0;

  for (long i = 1 ; i < 3 ; i += 1){
    if (colourDist[i] < colourDist[minDist]){
      minDist = i;
    }
  }
return minDist;
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

  int colourCode = findShortestDistance();
  return colourCode;
}
