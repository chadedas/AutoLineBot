int Kp = 20;
int Kd = 5;
int baseSpeed = 50;
int maxSpeed = 255;

int preError = 0;
int sumError = 0;

const int numSensors = 5;  // จำนวนเซนเซอร์
const int irSensorPins[numSensors] = {6, 5, 4, 3, 2};  // ขา Digital Input ที่เชื่อมต่อกับเซนเซอร์
int irSensorValues[numSensors];  // เก็บค่าที่ได้จากเซนเซอร์
int error = 0;  // เก็บค่า error

void setup() {
  Serial.begin(9600);  // เริ่มต้น Serial communication

  for (int i = 0; i < numSensors; i++) {
    pinMode(irSensorPins[i], INPUT);  // กำหนดขา Digital เป็นขานำเข้า
  }
}

void loop() {
  Serial.println("");
  for (int i = 0; i < numSensors; i++) {
    irSensorValues[i] = digitalRead(irSensorPins[i]);  // อ่านค่า Digital จากเซนเซอร์
  }
    Serial.print("State: ");
  for (int i = 0; i < numSensors; i++) {
    if (irSensorValues[i] == HIGH){
    irSensorValues[i] = 0;
    Serial.print(irSensorValues[i]);
    }else{
    irSensorValues[i] = 1;
    Serial.print(irSensorValues[i]);
    }
  }
     // เพื่อขึ้นบรรทัดใหม่ 
  Serial.println();  // เพื่อขึ้นบรรทัดใหม่

  if (irSensorValues[0] == LOW && irSensorValues[1] == LOW && irSensorValues[2] == LOW && irSensorValues[3] == LOW && irSensorValues[4] == HIGH) {
    error = 4;
          Serial.print("Error Value: ");
      Serial.println(error);
  } else if (irSensorValues[0] == LOW && irSensorValues[1] == LOW && irSensorValues[2] == LOW && irSensorValues[3] == HIGH && irSensorValues[4] == HIGH) {
    error = 3;
          Serial.print("Error Value: ");
      Serial.println(error);
  } else if (irSensorValues[0] == LOW && irSensorValues[1] == LOW && irSensorValues[2] == LOW && irSensorValues[3] == HIGH && irSensorValues[4] == LOW) {
    error = 2;
          Serial.print("Error Value: ");
      Serial.println(error);
  } else if (irSensorValues[0] == LOW && irSensorValues[1] == LOW && irSensorValues[2] == HIGH && irSensorValues[3] == HIGH && irSensorValues[4] == LOW) {
    error = 1;
          Serial.print("Error Value: ");
      Serial.println(error);
  } else if (irSensorValues[0] == LOW && irSensorValues[1] == LOW && irSensorValues[2] == HIGH && irSensorValues[3] == LOW && irSensorValues[4] == LOW) {
    error = 0;
          Serial.print("Error Value: ");
      Serial.println(error);
  } else if (irSensorValues[0] == LOW && irSensorValues[1] == HIGH && irSensorValues[2] == HIGH && irSensorValues[3] == LOW && irSensorValues[4] == LOW) {
    error = -1;
          Serial.print("Error Value: ");
      Serial.println(error);
  } else if (irSensorValues[0] == LOW && irSensorValues[1] == HIGH && irSensorValues[2] == LOW && irSensorValues[3] == LOW && irSensorValues[4] == LOW) {
    error = -2;
          Serial.print("Error Value: ");
      Serial.println(error);
  } else if (irSensorValues[0] == HIGH && irSensorValues[1] == HIGH && irSensorValues[2] == LOW && irSensorValues[3] == LOW && irSensorValues[4] == LOW) {
    error = -3;
          Serial.print("Error Value: ");
      Serial.println(error);
  } else if (irSensorValues[0] == HIGH && irSensorValues[1] == LOW && irSensorValues[2] == LOW && irSensorValues[3] == LOW && irSensorValues[4] == LOW) {
    error = -4;
      Serial.print("Error Value: ");
      Serial.println(error);
  } else {
    Serial.println("Error Value: No Vaule");
  }


 // คำนวณ PID และควบคุมความเร็วของมอเตอร์
  int speedChange = Kp * error + Kd * (error - preError);
  int leftSpeed = baseSpeed + speedChange;
  int rightSpeed = baseSpeed - speedChange;

  // จำกัดความเร็วให้อยู่ในช่วงที่กำหนด
  if (leftSpeed > maxSpeed) leftSpeed = maxSpeed;
  if (rightSpeed > maxSpeed) rightSpeed = maxSpeed;
  if (leftSpeed < -maxSpeed) leftSpeed = -maxSpeed;
  if (rightSpeed < -maxSpeed) rightSpeed = -maxSpeed;

  // แสดงค่าความเร็วทาง Serial Monitor
  Serial.print("Left Speed: ");
  Serial.println(leftSpeed);
  Serial.print("Right Speed: ");
  Serial.println(rightSpeed);
  delay(2000);
} 
