const int trigPin1 = 0;
const int echoPin1 = 23;
const int trigPin2 = 2;
const int echoPin2 = 25;
// length of robot in cm
#define ROBOTLEN      26 
// defines variables
long duration;
int distance;
void setupUltra() {
  DDRA = (1 << trigPin1);
  DDRA = (1 << trigPin2);
}

uint32_t findfrontUltra() {
  return measureUltra(trigPin1, echoPin1);
}

uint32_t findbackUltra() {
  return measureUltra(trigPin2, echoPin2);
}

uint32_t measureUltra(int trigPin, int echoPin) {
  // Clears the trigPin
  PORTA &= ~(1 << trigPin);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  PORTA |= (1 << trigPin);
  delayMicroseconds(10);
  PORTA &= ~(1 << trigPin);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  // Serial.print("Distance: ");
  // Serial.println(distance);
  return distance;
}