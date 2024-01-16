#include <L298N.h>
#include <L298NX2.h>

// Motor A pins (EN_A = enable motor, IN1_A = forward, IN2_A = backward)
int EN_A = 9;
int IN1_A = 7;
int IN2_A = 6;

//Motor B pins (enabledB = enable motor, IN2_B = forward, IN2_B = backward)
int EN_B = 10;
int IN1_B = 4;
int IN2_B = 5;
int P, D, I, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 200;

float Kp = 0;
float Kd = 0;
float Ki = 0 ;


int minValues[6], maxValues[6], threshold[6];

L298NX2 motors(EN_A, IN1_A, IN2_A, EN_B, IN1_B, IN2_B);
void setup()
{
  Serial.begin(9600);
}


void loop()
{
  calibrate();
  while (1)
  {
    if (analogRead(A1) > threshold[1] && analogRead(A5) < threshold[5] )
    {
      lsp = 0; rsp = lfspeed;
        motors.setSpeedA(0);
        motors.setSpeedB(lfspeed);
    }

    else if (analogRead(A5) > threshold[5] && analogRead(A1) < threshold[1])
    { 
      lsp = lfspeed; rsp = 0;
        motors.setSpeedA(lfspeed);
        motors.setSpeedB(0);
    }
    else if (analogRead(A3) > threshold[3])
    {
      Kp = 0.0006 * (1000 - analogRead(A3));
      Kd = 10 * Kp;
      //Ki = 0.0001;
      linefollow();
    }
  }
}

void linefollow()
{
  int error = (analogRead(A2) - analogRead(A4));

  P = error;
  I = I + error;
  D = error - previousError;

  PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
  previousError = error;

  lsp = lfspeed - PIDvalue;
  rsp = lfspeed + PIDvalue;

  if (lsp > 255) {
    lsp = 255;
  }
  if (lsp < 90 && lsp > 0) {
    lsp = 90;
  }
    if (lsp <= 0) {
    lsp = 0;
  }
  if (rsp > 255) {
    rsp = 255;
  }
  if (rsp < 90 && lsp > 0) {
    rsp = 90;
  }
    if (rsp <= 90 && rsp >= 0) {
    rsp = 0;
  }
          motors.setSpeedA(lsp);
        motors.setSpeedB(rsp);

}

void calibrate()
{
  for ( int i = 1; i < 6; i++)
  {
    minValues[i] = analogRead(A0 + i);
    maxValues[i] = analogRead(A0 + i);
  }
  
  for (int i = 0; i < 3000; i++)
  {
            motors.setSpeedA(100);
        motors.setSpeedB(-100);

    for ( int i = 1; i < 6; i++)
    {
      if (analogRead(A0 + i) < minValues[i])
      {
        minValues[i] = analogRead(A0 + i);
      }
      if (analogRead(A0 + i) > maxValues[i])
      {
        maxValues[i] = analogRead(A0 + i);
      }
    }
  }

  for ( int i = 1; i < 6; i++)
  {
    threshold[i] = (minValues[i] + maxValues[i]) / 2;
    Serial.print(threshold[i]);
    Serial.print("   ");
  }
  Serial.println();
          motors.setSpeedA(0);
        motors.setSpeedB(0);
}