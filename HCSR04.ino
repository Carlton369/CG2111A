// Pin Definitions
const int trigPin1 = 0;   // Trigger pin of ultrasonic sensor 1 22
const int echoPin1 = 1;  // Echo pin of ultrasonic sensor 1 23
const int trigPin2 = 2;  // Trigger pin of ultrasonic sensor 24
const int echoPin2 = 3;  // Echo pin of ultrasonic sensor 2 25

// Variables
volatile int distance_cm1, distance_cm2;

void HCSR04_setup() {  
  DDRA = 0b0101;   //configure input and output pins

  // Initialize trigger pins to low
  PORTA &= ~(1 << trigPin1);
  PORTA &= ~(1 << trigPin2);
  
}

void get_distance() {
  // Measure distance from sensor 1
  distance_cm1 = measureDistance(trigPin1, echoPin1);

  // Measure distance from sensor 2
  distance_cm2 = measureDistance(trigPin2, echoPin2);
  
}

// Function to measure distance from a sensor
int measureDistance(int trigPin, int echoPin) {
  // Send 10us pulse to trigger pin
  PORTA |= (1 << trigPin);
  delay(10);
  PORTA &= (1 << trigPin);

  // Measure the duration of the echo pulse
  int duration;
  duration = pulseIn(echoPin);

  // Calculate distance in centimeters
  return duration * 0.034 / 2;
}

// Function to measure pulse width
long pulseIn(int echoPin) {
  unsigned long start_time = micros();
  while ((PINA & (1 << echoPin)) == 0) {
    if ((micros() - start_time) > 8000) // Timeout after 8000 microseconds
      return 0;
  }
  start_time = micros();
  while ((PINA & (1 << echoPin)) != 0) {
    if ((micros() - start_time) > 8000) // Timeout after 8000 microseconds
      return 0;
  }
  return micros() - start_time;
}
