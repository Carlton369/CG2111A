// TCS230 or TCS3200 pins wiring to Arduino
#define S0 30
#define S1 31
#define S2 32
#define S3 33
#define sensorOut 40

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Stores the red. green and blue colors
int redColour = 0;
int greenColour = 0;
int blueColour = 0;

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
  delay(100);

  // Reading the output frequency for red
  redFrequency = avgFreq();
  redColour = map(redFrequency, 130, 860, 255, 0);
  delay(100);

  // Setting GREEN (G) filtered photodiodes to be read -------------------------
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  delay(100);

  // Reading the output frequency for green
  greenFrequency = avgFreq();
  greenColour = map(greenFrequency, 260, 1970, 255, 0);
  delay(100);

  // Setting BLUE (B) filtered photodiodes to be read ------------------------
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  delay(100);

  // Reading the output frequency for blue
  blueFrequency = avgFreq();
  blueColour = map(blueFrequency, 125, 950, 255, 0);
  delay(100);

  int colourCode;

  if (redColour > 280 && greenColour > 280 && blueColour > 280) {
    colourCode = 0;
  } else if (redColour > greenColour && redColour > blueColour) {

    colourCode = 1;  //0 represent red

  } else if (greenColour > redColour && greenColour > blueColour) {

    colourCode = 2;  //1 represent green

  } else {
    colourCode = 0;
  }

  return colourCode;
}

/*
void loop() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  
   // Printing the RED (R) value 58 328
  Serial.print("R = ");
  Serial.print(redFrequency);
  Serial.print("|");
  redFrequency = map(redFrequency, 130, 860, 255 ,0);
  Serial.print(redFrequency);
  delay(100);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the GREEN (G) value   20 760
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  Serial.print("|");
  greenFrequency = map(greenFrequency, 260, 1970, 255, 0);
  Serial.print(greenFrequency);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the BLUE (B) value 50 
  Serial.print(" B = ");
  Serial.print(blueFrequency);
  Serial.print("|");
  blueFrequency = map(blueFrequency, 125, 950, 255,0);
  Serial.print(blueFrequency);
  delay(100);

  if (redFrequency > 280 && greenFrequency > 280 && blueFrequency > 280) {
    Serial.print("white");
  }
  else if (redFrequency > blueFrequency && redFrequency > greenFrequency){
    Serial.print("red");
  }
  // else if (blueFrequency > redFrequency && blueFrequency > greenFrequency){
  //   Serial.print("blue");
  // }
  else if (greenFrequency > blueFrequency && greenFrequency > redFrequency){
    Serial.print("green");
  }
  else {
    Serial.print("white");
  }
  Serial.print("\n");
} */
