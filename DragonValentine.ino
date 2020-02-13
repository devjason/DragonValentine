#include <IRremote.h>
#include <Servo.h>


int LEFT_EYE = 5;
int RIGHT_EYE = 6;
const int EYE_TIME = 500;
const int FAST_EYE_TIME = 250;
bool lightEyes = true;
unsigned long lastEyeTime = 0;
unsigned long targetEyeTime = EYE_TIME;

const int IR_IN = 7;
IRrecv irRecv(IR_IN);
decode_results results;

int TAIL_PIN = 3;
Servo tail;
unsigned long tailStart = 0;

int NECK_PIN = 4;
Servo neck;
unsigned long neckStart = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(LEFT_EYE, OUTPUT);
  pinMode(RIGHT_EYE, OUTPUT);

  irRecv.enableIRIn();

  tail.attach(TAIL_PIN);
  neck.attach(NECK_PIN);
}

void loop() {
  unsigned long now = millis();
  digitalWrite(LEFT_EYE, lightEyes ? HIGH : LOW);
  digitalWrite(RIGHT_EYE, lightEyes ? HIGH : LOW);
  if ((now - lastEyeTime) > targetEyeTime) {
    lightEyes = !lightEyes;
    lastEyeTime = now;
  }

  if (irRecv.decode(&results)) {
    Serial.println(results.value, HEX);
    switch(results.value) {
      case 0xFFE01F:
        Serial.println("Tail left");
        tail.write(135);
        tailStart = now;
        targetEyeTime = FAST_EYE_TIME;
        break;
      case 0xFFA857:
        Serial.println("Tail right");
        tail.write(45);
        tailStart = now;
        targetEyeTime = FAST_EYE_TIME;
        break;
      case 0xFF22DD:
        Serial.println("Neck left");
        neck.write(160);
        neckStart = now;
        targetEyeTime = FAST_EYE_TIME;
        break;
      case 0xFF02FD:
        Serial.println("Tail right");
        neck.write(30);
        neckStart = now;
        targetEyeTime = FAST_EYE_TIME;
        break;
    }
    irRecv.resume();
  }

  if ((tailStart > 0) && (now - tailStart) > 750) {
    tail.write(90);
    tailStart = 0;
    targetEyeTime = EYE_TIME;
    Serial.println("Tail Halted");
  }

  if ((neckStart > 0) && (now - neckStart) > 750) {
    neck.write(90);
    neckStart = 0;
    targetEyeTime = EYE_TIME;
    Serial.println("Neck Halted");
  }
}
