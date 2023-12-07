const int numSensors = 5;  // จำนวนเซนเซอร์
const int irSensorPins[numSensors] = {2, 3, 4, 5, 6};  // ขา Digital Input ที่เชื่อมต่อกับเซนเซอร์

int Kp = 20;
int Kd = 5;
int baseSpeed = 50;
int maxSpeed = 255;

int preError = 0;
int sumError = 0;

void setup() {
  Serial.begin(9600);  // เริ่มต้น Serial communication

  for (int i = 0; i < numSensors; i++) {
    pinMode(irSensorPins[i], INPUT);  // กำหนดขา Digital เป็นขานำเข้า
  }
}

void loop() {
  int error = readSensorValues();  // อ่านค่าเซนเซอร์และคำนวณค่า error
  int motorSpeed = calculateMotorSpeed(error);  // คำนวณความเร็วของมอเตอร์
  controlMotors(motorSpeed);  // ควบคุมมอเตอร์
}

int readSensorValues() {
  int irSensorValues[numSensors];  // เก็บค่าที่ได้จากเซนเซอร์

  for (int i = 0; i < numSensors; i++) {
    irSensorValues[i] = digitalRead(irSensorPins[i]);  // อ่านค่า Digital จากเซนเซอร์
  }

  // ตรวจสอบค่าเซนเซอร์และคืนค่า error
  if (irSensorValues[0] == LOW && irSensorValues[1] == LOW && irSensorValues[2] == LOW && irSensorValues[3] == LOW && irSensorValues[4] == HIGH) {
    return 4;
  } else if (irSensorValues[0] == LOW && irSensorValues[1] == LOW && irSensorValues[2] == LOW && irSensorValues[3] == HIGH && irSensorValues[4] == HIGH) {
    return 3;
  } else if (irSensorValues[0] == LOW && irSensorValues[1] == LOW && irSensorValues[2] == LOW && irSensorValues[3] == HIGH && irSensorValues[4] == LOW) {
    return 2;
  } else if (irSensorValues[0] == LOW && irSensorValues[1] == LOW && irSensorValues[2] == HIGH && irSensorValues[3] == HIGH && irSensorValues[4] == LOW) {
    return 1;
  } else if (irSensorValues[0] == LOW && irSensorValues[1] == LOW && irSensorValues[2] == HIGH && irSensorValues[3] == LOW && irSensorValues[4] == LOW) {
    return 0;
  } else if (irSensorValues[0] == LOW && irSensorValues[1] == HIGH && irSensorValues[2] == HIGH && irSensorValues[3] == LOW && irSensorValues[4] == LOW) {
    return -1;
  } else if (irSensorValues[0] == LOW && irSensorValues[1] == HIGH && irSensorValues[2] == LOW && irSensorValues[3] == LOW && irSensorValues[4] == LOW) {
    return -2;
  } else if (irSensorValues[0] == HIGH && irSensorValues[1] == HIGH && irSensorValues[2] == LOW && irSensorValues[3] == LOW && irSensorValues[4] == LOW) {
    return -3;
  } else if (irSensorValues[0] == HIGH && irSensorValues[1] == LOW && irSensorValues[2] == LOW && irSensorValues[3] == LOW && irSensorValues[4] == LOW) {
    return -4;
  } else {
    Serial.println("ERROR!");  // หากไม่ตรงกับเงื่อนไขใด ๆ ให้แสดงข้อความ "ERROR!"
    return 0;
  }
}

int calculateMotorSpeed(int error) {
  int speedChange = Kp * error + Kd * (error - preError);
  return baseSpeed + speedChange;
}

void controlMotors(int motorSpeed) {
  int leftSpeed = motorSpeed;
  int rightSpeed = baseSpeed - motorSpeed;

  // กำหนดความเร็วสูงสุด
  leftSpeed = constrain(leftSpeed, -maxSpeed, maxSpeed);
  rightSpeed = constrain(rightSpeed, -maxSpeed, maxSpeed);

  // ควบคุมมอเตอร์
  // (ตัวอย่าง: ให้ใช้ฟังก์ชันที่คุณใช้ในการควบคุมมอเตอร์ของคุณ)
  // motorControlFunction(leftSpeed, rightSpeed);

  // บันทึกค่า error สำหรับการคำนวณในรอบถัดไป
  preError = error;
  sumError += error;
}
