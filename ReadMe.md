# Project เกมทายตัวเลข 
## เริ่มจากออกแบบเกม 
- สิ่งที่ต้องใช้
  - Keypad
  - Photo board
- การทำงานของเกม
  1. เกมจะรับ input จากผู้เล่น ที่keypad โดยผู้เล่นจะมีชีวิตอยู่ 6 ชีวิต สามารถใส่ทายตัวเลขได้เเค่ 6 ครั้ง
  2. ตัวเกมจะสุ่มตัวเลขไปเรื่อยๆ
  3. เมื่อผู้เล่น ลองใส่ตัวเลข ทายไปที่ละครั้งแล้วยังทายไม่ถูกจนครบจนหมดชีวิต ครบที่ 6 ครั้ง จะเท่ากับ "แพ้" แล้วเริ่มเหมใหม่
  4. แต่ถ้าหากชนะ ไฟที่บอร์ดจะกระพริบ
## Code

- ตั้งค่าต่างๆ
  ```bash
  bool isGameStarted = false; // ตัวแปรสำหรับตรวจสอบว่าเกมเริ่มแล้วหรือยัง
  int attemptsLeft = 6; // จำนวนครั้งที่ผู้เล่นทายได้
  int secretNumber; // ตัวแปรเก็บเลขลับ
  const int ledPin = 13; // ปรับให้ตรงกับหมายเลขขาของ LED ที่ใช้
  
  #define tKeyDelay 50
  int r1 = 21, r2 = 20, r3 = 19, r4 = 18;
  int c1 = 8, c2 = 9, c3 = 10;
  int r1State, r2State;
  int r3State, r4State;
  int colCnt, valKeyIn;
  unsigned int cpuTick;
  unsigned long lasttime;

- เป็นฟังก์ชันที่ใช้ในการนับ tick ของเวลาในหน่วยมิลลิวินาที (ms) โดยใช้ฟังก์ชัน millis() ของ Arduino หรือระบบที่คล้ายกัน ซึ่ง millis() จะคืนค่าเวลาที่ระบบทำงานมาตั้งแต่เริ่มต้นในหน่วยมิลลิวินาที
  ```bash
  void initTick() {
      cpuTick = 0;
      lasttime = millis();
  }
  
  void sysTick() {
      if (millis() - lasttime >= 1) {
          lasttime = millis();
          cpuTick++;
      }
  }
- ตั้งค่า pinMode keypad
  ```bash
  void initKeypad() {
      pinMode(c1, OUTPUT);
      pinMode(c2, OUTPUT);
      pinMode(c3, OUTPUT);
      pinMode(r1, INPUT_PULLUP);
      pinMode(r2, INPUT_PULLUP);
      pinMode(r3, INPUT_PULLUP);
      pinMode(r4, INPUT_PULLUP);
      colCnt = 0;
      valKeyIn = -1;
  }
- เรียกใช้ function ต่างๆ ที่ main
  ```bash
    void setup() {
        initTick();
        initKeypad();
        Serial.begin(9600);
        randomSeed(analogRead(0)); // ใช้แอนะล็อกอ่านค่าเพื่อ seed สำหรับ random
    }

void scanKey() {
    if (cpuTick >= tKeyDelay) {
        cpuTick = 0;
        if (colCnt == 0) scanC1();
        if (colCnt == 1) scanC2();
        if (colCnt == 2) scanC3();
        if (colCnt == 3) scanC4();
        colCnt++;
        if (colCnt > 4) colCnt = 0;
    }
}

void scanC1() {
    digitalWrite(c1, LOW);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    r1State = digitalRead(r1);
    r2State = digitalRead(r2);
    r3State = digitalRead(r3);
    r4State = digitalRead(r4);

}

void scanC2() {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, LOW);
    digitalWrite(c3, HIGH);
    r1State = digitalRead(r1);
    r2State = digitalRead(r2);
    r3State = digitalRead(r3);
    r4State = digitalRead(r4);

}

void scanC3() {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, LOW);
    r1State = digitalRead(r1);
    r2State = digitalRead(r2);
    r3State = digitalRead(r3);
    r4State = digitalRead(r4);

}

void scanC4() {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    r1State = digitalRead(r1);
    r2State = digitalRead(r2);
    r3State = digitalRead(r3);
    r4State = digitalRead(r4);

}

void loop() {
    sysTick();
    scanKey(); // เรียก scanKey() เพื่อสแกนการกดปุ่มจาก numpad

    // ส่วนนี้คือการเริ่มเกมใหม่เมื่อผู้เล่นกดปุ่ม '0'
    if (!isGameStarted) { // เช็คว่าเกมยังไม่เริ่ม
        if (checkKeypad() == 0) { // ถ้าผู้เล่นกด '0' ให้เริ่มเกม
            Serial.println("Welcome to Guess the Number Game!");
            Serial.println("I'm thinking of a number between 1 and 9.");
            Serial.println("Try to guess it!");

            secretNumber = random(1, 9); // สุ่มเลขใหม่
            attemptsLeft = 6; // รีเซ็ตจำนวนครั้งที่ทายได้เป็น 6 ครั้ง
            isGameStarted = true; // ตั้งค่าเกมเริ่มแล้ว
        }
    } else { // ถ้าเกมเริ่มแล้ว
        if (attemptsLeft > 0) { // ตรวจสอบว่าผู้เล่นยังทายได้อยู่หรือไม่
            int guess = checkKeypad(); // รับค่าจากการสแกน numpad
            if (guess >= 1 && guess <= 9) { // ตรวจสอบว่าผู้ใช้ใส่ตัวเลขระหว่าง 1 ถึง 9 หรือไม่

                // ตรวจสอบว่าเดาถูกหรือไม่
                if (guess == secretNumber) {
                    Serial.println("Congratulations! You guessed the number.");
                    for (int k = 0; k < 10; k++) {
                        digitalWrite(ledPin, HIGH); // เปิด LED
                        delay(400);
                        digitalWrite(ledPin, LOW);  // ปิด LED
                        delay(100);
                    }
                    Serial.println("Let's play again. Press 0 to start a new game.");
                    isGameStarted = false; // รีเซ็ตตัวแปรเพื่อให้ผู้เล่นสามารถเริ่มเกมใหม่ได้
                } else {
                    // ลบคะแนนเมื่อเดาผิด
                    Serial.print("Sorry, ");
                    if (guess < secretNumber) {
                        Serial.println("too low. Try again.");
                        delay(500);
                    } else {
                        Serial.println("too high. Try again.");
                        delay(500);
                    }

                    // ลดจำนวนครั้งที่ทายได้ลงทีละ 1 โดยใช้ attemptsLeft - 1
                    attemptsLeft = attemptsLeft - 1;

                    Serial.print("You have ");
                    Serial.print(attemptsLeft);
                    Serial.println(" attempts left.");
                }
            }
        } else {
            // เมื่อผู้เล่นทายครบ 6 ครั้งแล้วแต่ยังไม่ถูก
            Serial.println("Game over! You've used all your attempts.");
            Serial.println("Let's play again. Press 0 to start a new game.");
            isGameStarted = false; // รีเซ็ตตัวแปรเพื่อให้ผู้เล่นสามารถเริ่มเกมใหม่ได้
        }
    }

    delay(100); // เพิ่มดีเลย์ให้ลูปหยุดชั่วคราว เพื่อป้องกันการรันซ้ำอย่างรวดเร็ว
}

// ฟังก์ชันเช็คการกดปุ่มจาก numpad โดยตรง
int checkKeypad() {
    int keyPressed = -1; // ค่าเริ่มต้น ไม่มีการกดปุ่ม

    // ตรวจสอบคอลัมน์แรก
    digitalWrite(c1, LOW);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    if (digitalRead(r1) == LOW) keyPressed = 1;
    if (digitalRead(r2) == LOW) keyPressed = 4;
    if (digitalRead(r3) == LOW) keyPressed = 7;
    if (digitalRead(r4) == LOW) keyPressed = 13;

    // ตรวจสอบคอลัมน์ที่สอง
    digitalWrite(c1, HIGH);
    digitalWrite(c2, LOW);
    digitalWrite(c3, HIGH);
    if (digitalRead(r1) == LOW) keyPressed = 2;
    if (digitalRead(r2) == LOW) keyPressed = 5;
    if (digitalRead(r3) == LOW) keyPressed = 8;
    if (digitalRead(r4) == LOW) keyPressed = 0; // กด 0

    // ตรวจสอบคอลัมน์ที่สาม
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, LOW);
    if (digitalRead(r1) == LOW) keyPressed = 3;
    if (digitalRead(r2) == LOW) keyPressed = 6;
    if (digitalRead(r3) == LOW) keyPressed = 9;
    if (digitalRead(r4) == LOW) keyPressed = 14;

    return keyPressed; // ส่งค่าปุ่มที่กด
}
