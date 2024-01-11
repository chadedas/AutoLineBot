int Kp = 25;
int Kd = 0;
int Ki = 0;
int error = 0;
int preError = 0;
int sumError = 0;

int N = 5;
int lineV = 0;
int groundV = 0;
int meanV = 500;

int baseSpeed = 100;
int maxSpeed = 255;
int rightSpeed, leftSpeed;
int motorSpeed;

int motorLeft = 11;
int motorRight = 12;

bool B(int n) {
  if (n < meanV) {  // is black
    return true;
  } else {
    return false;
  }
}

bool W(int n) {
  if (n >= meanV) {  // is white
    return true;
  } else {
    return false;
  }
}

const int numSensors = 5;                                // จำนวนเซนเซอร์
const int irSensorPins[numSensors] = { 6, 5, 4, 3, 2 };  // ขา Digital Input ที่เชื่อมต่อกับเซนเซอร์
int irSensorValues[numSensors];                          // เก็บค่าที่ได้จากเซนเซอร์

void setup() {
  Serial.begin(9600);  // เริ่มต้น Serial communication

  for (int i = 0; i < numSensors; i++) {
    pinMode(irSensorPins[i], INPUT);  // กำหนดขา Digital เป็นขานำเข้า
  }
  pinMode(motorLeft, OUTPUT);
  pinMode(motorRight, OUTPUT);
}

void loop() {
  for (int i = 0; i < N; i++) {
    irSensorValues[i] = analogRead(i);
  }

  if (W(irSensorValues[0]) && W(irSensorValues[1]) && W(irSensorValues[2]) && W(irSensorValues[3]) && B(irSensorValues[4])) {
    error = 4;
  } else if (W(irSensorValues[0]) && W(irSensorValues[1]) && W(irSensorValues[2]) && B(irSensorValues[3]) && B(irSensorValues[4])) {
    error = 3;
  } else if (W(irSensorValues[0]) && W(irSensorValues[1]) && W(irSensorValues[2]) && B(irSensorValues[3]) && W(irSensorValues[4])) {
    error = 2;
  } else if (W(irSensorValues[0]) && W(irSensorValues[1]) && B(irSensorValues[2]) && B(irSensorValues[3]) && W(irSensorValues[4])) {
    error = 1;
  } else if (W(irSensorValues[0]) && W(irSensorValues[1]) && B(irSensorValues[2]) && W(irSensorValues[3]) && W(irSensorValues[4])) {
    error = 0;
  } else if (W(irSensorValues[0]) && B(irSensorValues[1]) && B(irSensorValues[2]) && W(irSensorValues[3]) && W(irSensorValues[4])) {
    error = -1;
  } else if (W(irSensorValues[0]) && B(irSensorValues[1]) && W(irSensorValues[2]) && W(irSensorValues[3]) && W(irSensorValues[4])) {
    error = -2;
  } else if (B(irSensorValues[0]) && B(irSensorValues[1]) && W(irSensorValues[2]) && W(irSensorValues[3]) && W(irSensorValues[4])) {
    error = -3;
  } else if (B(irSensorValues[0]) && W(irSensorValues[1]) && W(irSensorValues[2]) && W(irSensorValues[3]) && W(irSensorValues[4])) {
    error = -4;
  }
  /// check WWWWW
  else if (W(irSensorValues[0]) && W(irSensorValues[1]) && W(irSensorValues[2]) && W(irSensorValues[3]) && W(irSensorValues[4])) {
    error = preError;
  }
  if (irSensorValues[0] == LOW && irSensorValues[1] == LOW && irSensorValues[2] == LOW && irSensorValues[3] == LOW && irSensorValues[4] == LOW) {
  } else {
    Serial.print("Error Value: ");
    Serial.println(error);
  }
  // จำกัดความเร็วให้อยู่ในช่วงที่กำหนด

  motorSpeed = Kp * error + Kd * (error - preError) + Ki * (sumError);
  leftSpeed = baseSpeed + motorSpeed;
  rightSpeed = baseSpeed - motorSpeed;

  if (leftSpeed > maxSpeed) leftSpeed = maxSpeed;
  if (rightSpeed > maxSpeed) rightSpeed = maxSpeed;

  if (leftSpeed < -maxSpeed) leftSpeed = -maxSpeed;
  if (rightSpeed < -maxSpeed) rightSpeed = -maxSpeed;

  analogWrite(motorLeft, leftSpeed);
  analogWrite(motorRight, rightSpeed);
  delay(33);

  preError = error;
  sumError += error;

  //lcd("e=%d pe=%d|Ls=%d RS=%d|kp=%d kd=%d", error,preError, leftSpeed, rightSpeed ,Kp*error, Kd*(error - preError) );

  error = 0;
}
