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
int maxSpeed = 155;
int rightSpeed, leftSpeed;
int motorSpeed;

// Motor A
int dir1PinA = 2;
int dir2PinA = 3;
int motorLeft = 6;  //   เพื่อให้ PWM สามารถควบคุมความเร็วมอเตอร์

// Motor B

int dir1PinB = 4;
int dir2PinB = 5;
int motorRight = 7;  // เพื่อให้ PWM สามารถควบคุมความเร็วมอเตอร์


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
const int irSensorPins[numSensors] = { A5, A4, A3, A2, A1 };  // ขา Digital Input ที่เชื่อมต่อกับเซนเซอร์
int irSensorValues[numSensors];                          // เก็บค่าที่ได้จากเซนเซอร์

void setup() {
  Serial.begin(9600);  // เริ่มต้น Serial communication

  for (int i = 0; i < numSensors; i++) {
    pinMode(irSensorPins[i], INPUT);  // กำหนดขา Digital เป็นขานำเข้า
  }
  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(motorLeft, OUTPUT);
  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
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
  error = 0;
}
