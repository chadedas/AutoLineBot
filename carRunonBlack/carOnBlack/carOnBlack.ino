// Motor A pins (enableA = enable motor, pinA1 = forward, pinA2 = backward)
int enableA = 9;
int pinA1 = 7;
int pinA2 = 6;

//Motor B pins (enabledB = enable motor, pinB2 = forward, pinB2 = backward)
int enableB = 10;
int pinB1 = 4;
int pinB2 = 5;
int P, D, I, previousError, PIDvalue, error;
int lsp, rsp;
int lfspeed = 200;

float Kp = 0;
float Kd = 0;
float Ki = 0 ;


int minValues[6], maxValues[6], threshold[6];

void setup()
{
  Serial.begin(9600);
  pinMode(pinA1, OUTPUT);
  pinMode(pinA2, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(pinB1, OUTPUT);
  pinMode(pinB2, OUTPUT);
  pinMode(enableB, OUTPUT);
}


void loop()
{
  while (1)
  {
    if (analogRead(A1) > threshold[1] && analogRead(A5) < threshold[5] )
    {
      lsp = 0; rsp = lfspeed;
          analogWrite(enableA, 0);
    analogWrite(enableB, lfspeed);
    }

    else if (analogRead(A5) > threshold[5] && analogRead(A1) < threshold[1])
    { lsp = lfspeed; rsp = 0;
    analogWrite(enableA, lfspeed);
    analogWrite(enableB, 0);
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
  if (lsp < 0) {
    lsp = 0;
  }
  if (rsp > 255) {
    rsp = 255;
  }
  if (rsp < 0) {
    rsp = 0;
  }
    analogWrite(enableA, lsp);
    analogWrite(enableB, rsp);

}

void calibrate()
{
  for ( int i = 1; i < 6; i++)
  {
    minValues[i] = analogRead("A" + i);
    maxValues[i] = analogRead("A" + i);
  }
  
  for (int i = 0; i < 3000; i++)
  {
    analogWrite(enableA, 50);
    analogWrite(enableB, -50);

    for ( int i = 1; i < 6; i++)
    {
      if (analogRead("A" + i) < minValues[i])
      {
        minValues[i] = analogRead("A" + i);
      }
      if (analogRead("A" + i) > maxValues[i])
      {
        maxValues[i] = analogRead("A" + i);
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
  
    analogWrite(enableA, 0);
    analogWrite(enableB, 0);
}