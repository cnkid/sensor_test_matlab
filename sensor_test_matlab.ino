
#include <FreqCount.h>
int output_pin = 7;

void setup() {
  char a;
  Serial.begin(115200);
  pinMode(output_pin, OUTPUT);
  Serial.println('a');
  while (a != 'a') {
    a = Serial.read();
  }
}

void loop() {
  String choice = "";
  while (1) {
    choice = Serial.readString();
    if (choice.equals("pulse")) {
      selectmode();
    }
    if (choice.equals("cont")) {
      contmode();
    }
  }
}

void selectmode(void) {
  unsigned long b = 0;
  unsigned long startM = 0;
  unsigned long prevM = 0;
  long t1;
  long t2;
  long t3;
  long t4;
  long t5;
  long t6;
  long last;
  long timestamp;
  int count = 0;
  float measures[100];
  String matlab = "";
  String buff = "";


  while (1) {
    while (matlab.equals("")) {
      delay(50);
      matlab = Serial.readString();
    }
    int t1index = matlab.indexOf(';');
    int t2index = matlab.indexOf(';', t1index + 1);
    int t3index = matlab.indexOf(';', t2index + 1);
    int t4index = matlab.indexOf(';', t3index + 1);
    int t5index = matlab.indexOf(';', t4index + 1);
    int t6index = matlab.indexOf(';', t5index + 1);
    t1 = matlab.substring(0, t1index).toInt();
    t2 = matlab.substring(t1index + 1, t2index).toInt();
    t3 = matlab.substring(t2index + 1, t3index).toInt();
    t4 = matlab.substring(t3index + 1, t4index).toInt();
    t5 = matlab.substring(t4index + 1, t5index).toInt();
    t6 = matlab.substring(t5index + 1, matlab.length()).toInt();




    startM = millis();
    FreqCount.begin(10); // decides how many ms between reads
    while (1) {
      digitalWrite(output_pin, HIGH);
      delay(t1);
      digitalWrite(output_pin, LOW);
      delay(t2);
      digitalWrite(output_pin, HIGH);
      delay(t3);
      digitalWrite(output_pin, LOW);
      delay(t4);
      digitalWrite(output_pin, HIGH);
      prevM = millis();
      //while ((millis() - prevM) <= 500) {
      //}
      count = (t5 / 10);
      while (count > 0) {
        while(!FreqCount.available()){
          prevM = millis();
        }
        if (FreqCount.available()) {
          b = FreqCount.read() * 100;
          if (b < 0) {
            b = 0;
          }
          timestamp = millis() - startM;
          buff = buff + String(timestamp) + "," + String(b) + "|";
          count = count - 1;
        }
      }
      Serial.print(buff);
      digitalWrite(output_pin, LOW);
      prevM = millis();
      while (millis() - prevM < t6) {
        delay(1000);
        Serial.print("j,j|");
      }
      buff = "";
    }
  }
}


void contmode(void) {
  unsigned long b = 0;
  unsigned long startM = 0;
  unsigned long prevM = 0;
  long timestamp;
  String matlab = "";
  String buff = "";
  startM = millis();
  FreqCount.begin(1000);
  digitalWrite(output_pin, HIGH);
  while (1) {
    if (FreqCount.available()) {
      b = FreqCount.read();
      timestamp = millis() - startM;
      buff = buff + String(timestamp) + "," + String(b) + "|";
      Serial.print(buff);
      buff = "";
    }
  }
}
