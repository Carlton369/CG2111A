#include <serialize.h>
//#include "Arduino.h"
// #include <buffer.h>
#include <stdarg.h>
#include <math.h>
#include "packet.h"
#include "constants.h"

volatile TDirection dir; // = STOP;

#include <avr/interrupt.h>

/*
   Alex's configuration constants
*/

// Number of ticks per revolution from the
// wheel encoder.

#define COUNTS_PER_REV      4             //to adjust distance travelled by Alex

// Wheel circumference in cm.
// We will use this to calculate forward/backward distance traveled
// by taking revs * WHEEL_CIRC

#define WHEEL_CIRC          21

/*
      Alex's State Variables
*/


#define ALEX_LENGTH 26         //to adjust turning angle (lower means decrease turning angle)
#define ALEX_BREADTH 15       //original dimensions: 16 by 6

float AlexDiagonal = 0.0;
float AlexCirc = 0.0;

unsigned long deltaTicks;
unsigned long targetTicks;

// Store the ticks from Alex's left and
// right encoders.
volatile unsigned long leftForwardTicks;
volatile unsigned long leftReverseTicks;
volatile unsigned long rightForwardTicks;
volatile unsigned long rightReverseTicks;

// Store the revolutions on Alex's left
// and right wheels
volatile unsigned long leftRevs;
volatile unsigned long rightRevs;

//left and right encoders ticks for turning
volatile unsigned long leftForwardTicksTurns;
volatile unsigned long leftReverseTicksTurns;
volatile unsigned long rightForwardTicksTurns;
volatile unsigned long rightReverseTicksTurns;

// Forward and backward distance traveled
volatile unsigned long forwardDist;
volatile unsigned long reverseDist;

//keeping track of distance travelled
unsigned long deltaDist;
unsigned long newDist;


// Stores frequency read by the photodiodes
volatile signed int redFrequency = 0;
volatile signed int greenFrequency = 0;
volatile signed int blueFrequency = 0;

// Stores the red. green and blue colors
volatile signed int redColour = 0;
volatile signed int greenColour = 0;
volatile signed int blueColour = 0;

/*
   Alex Communication Routines.
*/

TResult readPacket(TPacket *packet)
{
  // Reads in data from the serial port and
  // deserializes it.Returns deserialized
  // data in "packet".

  char buffer[PACKET_SIZE];
  int len;

  len = readSerial(buffer);

  if (len == 0)
    return PACKET_INCOMPLETE;
  else
    return deserialize(buffer, len, packet);

}

void sendStatus()
{
  // Implement code to send back a packet containing key
  // information like leftTicks, rightTicks, leftRevs, rightRevs
  // forwardDist and reverseDist
  // Use the params array to store this information, and set the
  // packetType and command files accordingly, then use sendResponse
  // to send out the packet. See sendMessage on how to use sendResponse.
  //
  TPacket statusPacket;
  statusPacket.packetType = PACKET_TYPE_RESPONSE;
  statusPacket.command = RESP_STATUS;
  statusPacket.params[0] = redFrequency;
  statusPacket.params[1] = greenFrequency;
  statusPacket.params[2] = blueFrequency;
  statusPacket.params[3] = redColour;
  statusPacket.params[4] = greenColour;
  statusPacket.params[5] = blueColour;
  statusPacket.params[6] = leftReverseTicksTurns;
  statusPacket.params[7] = rightReverseTicksTurns;
  statusPacket.params[8] = forwardDist;
  statusPacket.params[9] = reverseDist;
  statusPacket.params[10] = findfrontUltra();
  statusPacket.params[11] = findbackUltra();
  statusPacket.params[12] = findColour();
  sendResponse(&statusPacket);

}

void sendMessage(const char *message)
{
  // Sends text messages back to the Pi. Useful
  // for debugging.

  TPacket messagePacket;
  messagePacket.packetType = PACKET_TYPE_MESSAGE;
  strncpy(messagePacket.data, message, MAX_STR_LEN);
  sendResponse(&messagePacket);
}

void dbprintf(char* format, ...) {
  va_list args;
  char buffer[128];

  va_start (args, format);
  vsprintf(buffer, format, args);
  sendMessage(buffer);
}


void sendBadPacket()
{
  // Tell the Pi that it sent us a packet with a bad
  // magic number.

  TPacket badPacket;
  badPacket.packetType = PACKET_TYPE_ERROR;
  badPacket.command = RESP_BAD_PACKET;
  sendResponse(&badPacket);

}

void sendBadChecksum()
{
  // Tell the Pi that it sent us a packet with a bad
  // checksum.

  TPacket badChecksum;
  badChecksum.packetType = PACKET_TYPE_ERROR;
  badChecksum.command = RESP_BAD_CHECKSUM;
  sendResponse(&badChecksum);
}

void sendBadCommand()
{
  // Tell the Pi that we don't understand its
  // command sent to us.

  TPacket badCommand;
  badCommand.packetType = PACKET_TYPE_ERROR;
  badCommand.command = RESP_BAD_COMMAND;
  sendResponse(&badCommand);

}

void sendBadResponse()
{
  TPacket badResponse;
  badResponse.packetType = PACKET_TYPE_ERROR;
  badResponse.command = RESP_BAD_RESPONSE;
  sendResponse(&badResponse);
}

void sendOK()
{
  TPacket okPacket;
  okPacket.packetType = PACKET_TYPE_RESPONSE;
  okPacket.command = RESP_OK;
  sendResponse(&okPacket);
}

void sendResponse(TPacket *packet)
{
  // Takes a packet, serializes it then sends it out
  // over the serial port.
  char buffer[PACKET_SIZE];
  int len;

  len = serialize(buffer, packet, sizeof(TPacket));
  writeSerial(buffer, len);
}


/*
   Setup and start codes for external interrupts and
   pullup resistors.

*/
// Enable pull up resistors on pins 2 and 3
void enablePullups()
{
  // Use bare-metal to enable the pull-up resistors on pins
  // 2 and 3. These are pins PD2 and PD3 respectively.
  // We set bits 2 and 3 in DDRD to 0 to make them inputs.
  DDRD &= ~(1 << 2) & ~(1 << 3);
  PORTD |= (1 << 2) | (1 << 3);
}

// Functions to be called by INT0 and INT1 ISRs.
void leftISR()
{
  if (dir == FORWARD) {
    leftForwardTicks++;
    forwardDist = (unsigned long) ((float) leftForwardTicks / COUNTS_PER_REV * WHEEL_CIRC);
  }
  if (dir == BACKWARD) {
    leftReverseTicks++;
    reverseDist = (unsigned long) ((float) leftReverseTicks / COUNTS_PER_REV * WHEEL_CIRC);
  }
  if (dir == LEFT) {
    leftReverseTicksTurns++;
  }
  if (dir == RIGHT) {
    leftForwardTicksTurns++;
  }
  /*Serial.print("LEFT: ");
    Serial.println(leftTicks);
    leftRevs = leftTicks / COUNTS_PER_REV;
    Serial.println(leftRevs);*/
  //dbprintf("Left forward ticks: %i\n", leftForwardTicks);
  //dbprintf("Left reverse ticks: %i\n", leftReverseTicks);
}

void rightISR()
{
  if (dir == FORWARD) {
    rightForwardTicks++;
  }
  if (dir == BACKWARD) {
    rightReverseTicks++;
  }
  if (dir == LEFT) {
    rightForwardTicksTurns++;
  }
  if (dir == RIGHT) {
    rightReverseTicksTurns++;
  }
  /*Serial.print("RIGHT: ");
    Serial.println(rightTicks);
    rightRevs = rightTicks / COUNTS_PER_REV;
    Serial.println(rightRevs);*/
  //dbprintf("Right forward ticks: %i\n", rightForwardTicks);
  //dbprintf("Right reverse ticks: %i\n", rightReverseTicks);
}

// Set up the external interrupt pins INT0 and INT1
// for falling edge triggered. Use bare-metal.
void setupEINT()
{
  // Use bare-metal to configure pins 2 and 3 to be
  // falling edge triggered. Remember to enable
  // the INT0 and INT1 interrupts.
  EICRA = 0b00001010;
  EIMSK = 0b00000011;
  sei();
}

// Implement the external interrupt ISRs below.
// INT0 ISR should call leftISR while INT1 ISR
// should call rightISR.
ISR(INT0_vect) {
  leftISR();
}

ISR(INT1_vect) {
  rightISR();
}

// Implement INT0 and INT1 ISRs above.

/*
   Setup and start codes for serial communications

*/
// Set up the serial connection. For now we are using
// Arduino Wiring, you will replace this later
// with bare-metal code.
void setupSerial()
{
  // To replace later with bare-metal.
  Serial.begin(9600);
}

// Start the serial connection. For now we are using
// Arduino wiring and this function is empty. We will
// replace this later with bare-metal code.

void startSerial()
{
  // Empty for now. To be replaced with bare-metal code
  // later on.

}

// Read the serial port. Returns the read character in
// ch if available. Also returns TRUE if ch is valid.
// This will be replaced later with bare-metal code.

int readSerial(char *buffer)
{

  int count = 0;

  while (Serial.available())
    buffer[count++] = Serial.read();

  return count;
}

// Write to the serial port. Replaced later with
// bare-metal code

void writeSerial(const char *buffer, int len)
{
  Serial.write(buffer, len);
}

// Convert percentages to PWM values
/*
int pwmVal(float speed)
{
  if (speed < 0.0)
    speed = 0;

  if (speed > 100.0)
    speed = 100.0;

  return (int) ((speed / 100.0) * 255.0);
}
*/
unsigned long computeDeltaTicks(float ang) {
  unsigned long ticks = (unsigned long) ((ang * AlexCirc * COUNTS_PER_REV) / (360.0 * WHEEL_CIRC));
  return ticks;
}

// Turn Alex left "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn left indefinitely.


// Turn Alex right "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn right indefinitely.
/*void right(float ang, float speed)
{
  if (ang == 0) {
    deltaTicks = 9999999;
  } else {
    deltaTicks = computeDeltaTicks(ang);
  }
  targetTicks = rightReverseTicksTurns + deltaTicks;

  dir = RIGHT;
  int val = pwmVal(speed);

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn right we reverse the right wheel and move
  // the left wheel forward.
  analogWrite(RR, val);
  analogWrite(LF, val);
  analogWrite(LR, 0);
  analogWrite(RF, 0);
}*/


// Clears all our counters
void clearCounters()
{
  leftForwardTicks = 0;
  leftReverseTicks = 0;
  rightForwardTicks = 0;
  rightReverseTicks = 0;
  leftForwardTicksTurns = 0;
  rightForwardTicksTurns = 0;
  leftReverseTicksTurns = 0;
  rightReverseTicksTurns = 0;
  //leftRevs = 0;
  //rightRevs = 0;
  forwardDist = 0;
  reverseDist = 0;
}

// Clears one particular counter
void clearOneCounter(int which)
{
  clearCounters();
}

void initializeState()
{
  clearCounters();
}

void handleCommand(TPacket *command)
{
  switch (command->command)
  {
    // For movement commands, param[0] = distance, param[1] = speed.
    case COMMAND_FORWARD:
      sendOK();
      forward((float) command->params[0], (float) command->params[1]);
      //dbprintf("In forward param 0 is %f param 1 is %f\n", (float) command->params[0], (float) command->params[1]);
      break;

    case COMMAND_REVERSE:
      sendOK();
      backward((float) command->params[0], (float) command->params[1]);
      break;

    case COMMAND_TURN_LEFT:
      sendOK();
      ccw((float) command->params[0], (float) command->params[1]);
      break;

    case COMMAND_TURN_RIGHT:
      sendOK();
      cw((float) command->params[0], (float) command->params[1]);
      break;

    case COMMAND_STOP:
      sendOK();
      stop();
      break;
/*
    case COMMAND_COLOUR:                         //COLOUR SENSOR
      sendOK();
//      sendColour();
      break;

    case COMMAND_ULTRA:                         //ULTRASONIC SENSOR
      sendOK();
//      sendUltra();
      break;
*/
    case COMMAND_GET_STATS:
      sendStatus();
      break;

    case COMMAND_CLEAR_STATS:
      clearOneCounter(command->params[0]);
      blank();
      sendOK();
      break;

    default:
      sendBadCommand();
  }
}

void waitForHello()
{
  int exit = 0;

  while (!exit)
  {
    TPacket hello;
    TResult result;

    do
    {
      result = readPacket(&hello);
    } while (result == PACKET_INCOMPLETE);

    if (result == PACKET_OK)
    {
      if (hello.packetType == PACKET_TYPE_HELLO)
      {


        sendOK();
        exit = 1;
      }
      else
        sendBadResponse();
    }
    else if (result == PACKET_BAD)
    {
      sendBadPacket();
    }
    else if (result == PACKET_CHECKSUM_BAD)
      sendBadChecksum();
  } // !exit
}

void setup() {
  // put your setup code here, to run once:
  AlexDiagonal = sqrt((ALEX_LENGTH * ALEX_LENGTH) + (ALEX_BREADTH *
                      ALEX_BREADTH));
  AlexCirc = PI  * AlexDiagonal;


  cli();
  setupEINT();
  setupSerial();
  startSerial();
  setupColour();
  setupUltra();
  setupLED();
  enablePullups();
  initializeState();
  sei();
}

void handlePacket(TPacket *packet)
{
  switch (packet->packetType)
  {
    case PACKET_TYPE_COMMAND:
      handleCommand(packet);
      break;

    case PACKET_TYPE_RESPONSE:
      break;

    case PACKET_TYPE_ERROR:
      break;

    case PACKET_TYPE_MESSAGE:
      break;

    case PACKET_TYPE_HELLO:
      break;
  }
}

void loop() {

  
  // put your main code here, to run repeatedly:
  TPacket recvPacket; // This holds commands from the Pi

  TResult result = readPacket(&recvPacket);

  if (result == PACKET_OK)
    handlePacket(&recvPacket);
  else if (result == PACKET_BAD)
  {
    sendBadPacket();
  }
  else if (result == PACKET_CHECKSUM_BAD)
  {
    sendBadChecksum();
  }

  if (deltaDist > 0) {
    if (dir == FORWARD) {
      if (forwardDist > newDist) {
        deltaDist = 0;
        newDist = 0;
        stop();
      }
    } else if (dir == BACKWARD) {
      if (reverseDist > newDist) {
        deltaDist = 0;
        newDist = 0;
        stop();
      }
    } else if (dir == (TDirection) STOP) {
      deltaDist = 0;
      newDist = 0;
      stop();
    }
  }

  if (deltaTicks > 0) {
    if (dir == LEFT) {
      if (leftReverseTicksTurns >= targetTicks) {
        deltaTicks = 0;
        targetTicks = 0;
        stop();
      }
    } else if (dir == RIGHT) {
      if (rightReverseTicksTurns >= targetTicks) {
        deltaTicks = 0;
        targetTicks = 0;
        stop();
      }
    } else if (dir == (TDirection) STOP) {
      deltaTicks = 0;
      targetTicks = 0;
      stop();
    }
  }
}
