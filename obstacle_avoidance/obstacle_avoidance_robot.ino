// Motor driver pins
#define ENA 9
#define ENB 10
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

// Ultrasonic sensor pins (HC-SR04)
#define TRIG_PIN 11
#define ECHO_PIN 12

// Timeout for ultrasonic echo (to avoid blocking)
const unsigned long PULSE_TIMEOUT = 30000UL;

// Function prototypes
void forward(int, int);
void left(int, int);
void stopMotors();
long readUltrasonicCM();

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

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

// Move robot forward
void forward(int speed1, int speed2) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, constrain(speed1, 0, 255));
  analogWrite(ENB, constrain(speed2, 0, 255));
}

// Turn robot left to avoid obstacle
void left(int speed1, int speed2) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, constrain(speed1, 0, 255));
  analogWrite(ENB, constrain(speed2, 0, 255));
}

// Stop both motors
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// Measure distance using ultrasonic sensor (cm)
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
