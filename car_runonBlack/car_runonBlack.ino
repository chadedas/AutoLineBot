int Kp = 0.006;
int Kd = 0;
int Ki = 0;
int error = 0;
int preError = 0;
int sumError = 0;

int N = 5;
int lineV = 0;
int groundV = 0;
int meanV = 500;

int baseSpeed = 90;
int maxSpeed = 155;
int rightSpeed, leftSpeed;
int motorSpeed;

// Motor A
int dir1PinA = 7;
int dir2PinA = 6;
int motorLeft = 9;  //   เพื่อให้ PWM สามารถควบคุมความเร็วมอเตอร์

// Motor B

int dir1PinB = 4;
int dir2PinB = 5;
int motorRight = 10;  // เพื่อให้ PWM สามารถควบคุมความเร็วมอเตอร์


// ติด
bool W(int n) {
  if (n < meanV) {  // is black
 
    return true;
  } else {
   
    return false;
  }
}

//ดับ
bool B(int n) {
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
  analogWrite(motorLeft, 0);
  analogWrite(motorRight, 0);
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
if (W(irSensorValues[0]) && W(irSensorValues[1]) && W(irSensorValues[2]) && W(irSensorValues[3]) && W(irSensorValues[4])) {
      analogWrite(motorLeft, 0);
    analogWrite(motorRight, 0);
  if (B(irSensorValues[0]) && B(irSensorValues[1]) && B(irSensorValues[2]) && B(irSensorValues[3]) && B(irSensorValues[4])) {
    analogWrite(motorLeft, 0);
    analogWrite(motorRight, 0);

  
  }
  }else{
 motorSpeed = Kp * error;
  leftSpeed = baseSpeed + motorSpeed;
  rightSpeed = baseSpeed - motorSpeed;

  if (leftSpeed > maxSpeed) leftSpeed = maxSpeed;
  if (rightSpeed > maxSpeed) rightSpeed = maxSpeed;

  if (leftSpeed < baseSpeed) leftSpeed = baseSpeed;
  if (rightSpeed < baseSpeed) leftSpeed = baseSpeed;
  analogWrite(motorLeft, leftSpeed);
    digitalWrite(dir1PinA, HIGH);

  digitalWrite(dir2PinA, LOW);

  analogWrite(motorRight, rightSpeed);
    digitalWrite(dir1PinB, HIGH);

  digitalWrite(dir2PinB, LOW);
  delay(33);
  }


  // จำกัดความเร็วให้อยู่ในช่วงที่กำหนด

 

  preError = error;
  sumError += error;
  error = 0;
}
