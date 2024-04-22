#include <AFMotor.h>

// Motor control
#define FRONT_LEFT   4 // M4 on the driver shield
#define FRONT_RIGHT  1 // M1 on the driver shield
#define BACK_LEFT    3 // M3 on the driver shield
#define BACK_RIGHT   2 // M2 on the driver shield

AF_DCMotor motorFL(FRONT_LEFT);
AF_DCMotor motorFR(FRONT_RIGHT);
AF_DCMotor motorBL(BACK_LEFT);
AF_DCMotor motorBR(BACK_RIGHT);

void move(float speed, int direction)
{
  int speed_scaled = (speed/100.0) * 255;
  motorFL.setSpeed(speed_scaled);
  motorFR.setSpeed(speed_scaled);
  motorBL.setSpeed(speed_scaled);
  motorBR.setSpeed(speed_scaled);

  switch(direction)
    {
      case BACK:
        motorFL.run(BACKWARD);
        motorFR.run(BACKWARD);
        motorBL.run(FORWARD);
        motorBR.run(FORWARD); 
      break;
      case GO:
        motorFL.run(FORWARD);
        motorFR.run(FORWARD);
        motorBL.run(BACKWARD);
        motorBR.run(BACKWARD); 
      break;
      case CW:
        motorFL.run(BACKWARD);
        motorFR.run(FORWARD);
        motorBL.run(FORWARD);
        motorBR.run(BACKWARD); 
      break;
      case CCW:
        motorFL.run(FORWARD);
        motorFR.run(BACKWARD);
        motorBL.run(BACKWARD);
        motorBR.run(FORWARD); 
      break;
      case STOP:
      default:
        motorFL.run(STOP);
        motorFR.run(STOP);
        motorBL.run(STOP);
        motorBR.run(STOP); 
    }
}

void forward(float time_in, float speed)
{
  dir = (TDirection) FORWARD;
 if (time_in == 7 && speed == 7){
    move(100, FORWARD);
    delay(250);
    stop();
  } else {
  move(speed, FORWARD);
  /*
  if (dist > 0) deltaDist = dist;
  else deltaDist = 9999999;
  newDist = forwardDist + deltaDist;
  */
  delay(time_in);
  stop();
  }
}

void backward(float time_in, float speed)
{
  dir = (TDirection) BACKWARD;
  if (time_in == 7 && speed == 7){
    move(100, BACKWARD);
    delay(250);
    stop();
  } else {
  move(speed, BACKWARD);
  delay(time_in);
  stop();
  }
/*
  if (dist > 0) deltaDist = dist;
  else deltaDist = 9999999;
  newDist = reverseDist + deltaDist;
  */
}

void ccw(float time_in, float speed)
{
  dir = (TDirection) LEFT;
  if (time_in == 7 && speed == 7){
    move(100, CCW);
    delay(500);
    stop();
  } else {
  move(speed, CCW);
  delay(time_in);
  stop();
  }
}

void cw(float time_in, float speed)
{
  dir = (TDirection) RIGHT;
  if (time_in == 7 && speed == 7){
    move(100, CW);
    delay(500);
    stop();
  } else {
  move(speed, CW);
  delay(time_in);
  stop();
  }
}

void stop()
{
  dir = (TDirection) STOP;
  move(0, STOP);
}
