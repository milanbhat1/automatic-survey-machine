// Motor pins
#define ENA 9
#define ENB 10
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

// Ultrasonic sensor pins
#define TRIG_PIN 11
#define ECHO_PIN 12

const unsigned long PULSE_TIMEOUT = 30000UL;

void setup() {
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  long distance = readUltrasonicCM();

  if (distance > 20 || distance == -1) {
    forward(100, 100);
  } else {
    left(100, 100);
  }
}

// Motor control functions
void forward(int s1, int s2) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  analogWrite(ENA, s1); analogWrite(ENB, s2);
}

void left(int s1, int s2) {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, s1); analogWrite(ENB, s2);
}

long readUltrasonicCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, PULSE_TIMEOUT);
  if (duration == 0) return -1;
  return duration / 58;
}
