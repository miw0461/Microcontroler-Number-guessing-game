//Ex06-03: 4x4 keypad keymapping
#define tKeyDelay 50
int r1=21,r2=20,r3=19,r4=18;
int c1=8,c2=9,c3=10,c4=11;
int r1State,r2State;
int r3State,r4State;
int colCnt,valKeyIn;
unsigned int cpuTick;
unsigned long lasttime;

void initTick() {
    cpuTick = 0;
    lasttime = millis();
}

void sysTick() {
    if(millis()-lasttime >= 1) {
        lasttime = millis();
        cpuTick++;
    }
}

void initKeypad() {
    pinMode(c1, OUTPUT);
    pinMode(c2, OUTPUT);
    pinMode(c3, OUTPUT);
    pinMode(c4, OUTPUT);
    pinMode(r1, INPUT_PULLUP);
    pinMode(r2, INPUT_PULLUP);
    pinMode(r3, INPUT_PULLUP);
    pinMode(r4, INPUT_PULLUP);
    colCnt = 0;
    valKeyIn = -1;
}

void setup() {
    initTick();
    initKeypad();
    Serial.begin(9600);
}

void loop() {
    sysTick();
    scanKey();
    showValue();
}

void scanKey() {
    unsigned char k;
    if(cpuTick >= tKeyDelay) {
        cpuTick = 0;
        if(colCnt == 0) scanC1();
        if(colCnt == 1) scanC2();  
        if(colCnt == 2) scanC3();
        if(colCnt == 3) scanC4();   

        colCnt++;
        if(colCnt>4) colCnt = 0;
    }
}

void scanC1() {
    digitalWrite(c1, LOW);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
    r1State = digitalRead(r1);
    r2State = digitalRead(r2);
    r3State = digitalRead(r3);
    r4State = digitalRead(r4); 

    if(r1State == LOW) valKeyIn = 1; 
    if(r2State == LOW) valKeyIn = 4;
    if(r3State == LOW) valKeyIn = 7;
    if(r4State == LOW) valKeyIn = 13;
}

void scanC2() {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, LOW);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
    r1State = digitalRead(r1);
    r2State = digitalRead(r2);
    r3State = digitalRead(r3);
    r4State = digitalRead(r4); 

    if(r1State == LOW) valKeyIn = 2;
    if(r2State == LOW) valKeyIn = 5;
    if(r3State == LOW) valKeyIn = 8;
    if(r4State == LOW) valKeyIn = 0;
}

void scanC3() {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, LOW);
    digitalWrite(c4, HIGH);
    r1State = digitalRead(r1);
    r2State = digitalRead(r2);
    r3State = digitalRead(r3);
    r4State = digitalRead(r4); 

    if(r1State == LOW) valKeyIn = 3;
    if(r2State == LOW) valKeyIn = 6;
    if(r3State == LOW) valKeyIn = 9;
    if(r4State == LOW) valKeyIn = 14;
}

void scanC4() {
    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, LOW);
    r1State = digitalRead(r1);
    r2State = digitalRead(r2);
    r3State = digitalRead(r3);
    r4State = digitalRead(r4); 

    if(r1State == LOW) valKeyIn = 10;
    if(r2State == LOW) valKeyIn = 11;
    if(r3State == LOW) valKeyIn = 12;
    if(r4State == LOW) valKeyIn = 15;
}

void showValue() {
    if(valKeyIn > -1) {
        switch(valKeyIn) {
            case 0: Serial.println("butt 0 press, 0"); break;
            case 1: Serial.println("butt 1 press, 1"); break;
            case 2: Serial.println("butt 2 press, 2"); break;
            case 3: Serial.println("butt 3 press, 3"); break;
            case 4: Serial.println("butt 4 press, 4"); break;
            case 5: Serial.println("butt 5 press, 5"); break;
            case 6: Serial.println("butt 6 press, 6"); break;
            case 7: Serial.println("butt 7 press, 7"); break;
            case 8: Serial.println("butt 8 press, 8"); break;
            case 9: Serial.println("butt 9 press, 9"); break;
            case 10: Serial.println("butt + press, 10"); break;
            case 11: Serial.println("butt - press, 11"); break;  
            case 12: Serial.println("butt / press, 12"); break;
            case 13: Serial.println("butt * press, 13"); break; 
            case 14: Serial.println("butt = press, 14"); break;
            case 15: Serial.println("butt press, 15"); break;  
        }
        valKeyIn = -1;
    }
}