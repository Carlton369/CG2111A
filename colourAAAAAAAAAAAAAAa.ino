// TCS230 or TCS3200 pins wiring to Arduino
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// Stores frequency read by the photodiodes
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

void setup() {
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
   // Begins serial communication 
  Serial.begin(9600);
}
void loop() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  
   // Printing the RED (R) value 58 328
  Serial.print("R = ");
  //Serial.print(redFrequency);
  //Serial.print("|");
  redFrequency = map(redFrequency, 224, 630 , 255 ,0);
  Serial.print(redFrequency);
  delay(100);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the GREEN (G) value   20 760
  Serial.print(" G = ");
  //Serial.print(greenFrequency);
  //Serial.print("|");
  greenFrequency = map(greenFrequency, 540 ,800,255,0);
  Serial.print(greenFrequency);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the BLUE (B) value 50 
  Serial.print(" B = ");
  //Serial.print(blueFrequency);
  //Serial.print("|");
  blueFrequency = map(blueFrequency, 760 , 1000 ,255,0);
  Serial.print(blueFrequency);
  delay(100);

  if (redFrequency > blueFrequency && redFrequency > greenFrequency){
    Serial.print("red");
  }
  else if (blueFrequency > redFrequency && blueFrequency > greenFrequency){
    Serial.print("blue");
  }
  else if (greenFrequency > blueFrequency && greenFrequency > redFrequency){
    Serial.print("green");
  }
  else {
    Serial.print("what");
  }
  Serial.print("\n");
}