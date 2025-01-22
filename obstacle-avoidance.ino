// Define ultrasonic sensor pins
const int trigPinRight = 8; // Trigger pin for the right sensor
const int echoPinRight = 9; // Echo pin for the right sensor

const int trigPinCenter = 10; // Trigger pin for the center sensor
const int echoPinCenter = 11; // Echo pin for the center sensor

const int trigPinLeft = 12; // Trigger pin for the left sensor
const int echoPinLeft = 13; // Echo pin for the left sensor

// Define motor driver pins
const int motor1Forward = 2;  // Motor 1 forward
const int motor1Backward = 3; // Motor 1 backward
const int motor2Forward = 4;  // Motor 2 forward
const int motor2Backward = 5; // Motor 2 backward

// Define constants
const int safeDistance = 5;  // Safe distance in centimeters
const int checkInterval = 50; // Time interval to check for obstacles (ms)

// Variables for timing
unsigned long previousMillis = 0;

void setup() {
  // Set ultrasonic sensor pins as input/output
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);

  pinMode(trigPinCenter, OUTPUT);
  pinMode(echoPinCenter, INPUT);

  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);

  // Set motor driver pins as outputs
  pinMode(motor1Forward, OUTPUT);
  pinMode(motor1Backward, OUTPUT);
  pinMode(motor2Forward, OUTPUT);
  pinMode(motor2Backward, OUTPUT);

  // Begin serial communication for debugging
  Serial.begin(9600);
}

void loop() {
  // Current time for obstacle checking
  unsigned long currentMillis = millis();

  // Check for obstacles only if the interval has passed
  if (currentMillis - previousMillis >= checkInterval) {
    previousMillis = currentMillis;

    // Measure distances from the ultrasonic sensors
    int distanceCenter = measureDistance(trigPinCenter, echoPinCenter);
    int distanceRight = measureDistance(trigPinRight, echoPinRight);
    int distanceLeft = measureDistance(trigPinLeft, echoPinLeft);

    // Debugging: Print distances to Serial Monitor
    Serial.print("Center: ");
    Serial.print(distanceCenter);
    Serial.print(" cm, Right: ");
    Serial.print(distanceRight);
    Serial.print(" cm, Left: ");
    Serial.println(distanceLeft);

    // Logic for obstacle avoidance
    if (distanceCenter < safeDistance) {
      // Obstacle in front - check left and right
      if (distanceRight > distanceLeft && distanceRight > safeDistance) {
        // Turn right if the right side is clear
        moveRight();
      } else if (distanceLeft > safeDistance) {
        // Turn left if the left side is clear
        moveLeft();
      } else {
        // If both sides are blocked, move backward
        moveBackward();
        delay(500); // Delay to create some space
      }
    } else {
      // No obstacle in front, keep moving forward
      moveForward();
    }
  }
}

// Function to measure distance using an ultrasonic sensor
int measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Convert duration to centimeters
  return distance;
}

// Functions to control motor movements
void moveForward() {
  digitalWrite(motor1Forward, HIGH);
  digitalWrite(motor1Backward, LOW);
  digitalWrite(motor2Forward, HIGH);
  digitalWrite(motor2Backward, LOW);
}

void moveBackward() {
  digitalWrite(motor1Forward, LOW);
  digitalWrite(motor1Backward, HIGH);
  digitalWrite(motor2Forward, LOW);
  digitalWrite(motor2Backward, HIGH);
}

void moveLeft() {
  digitalWrite(motor1Forward, LOW);
  digitalWrite(motor1Backward, HIGH);
  digitalWrite(motor2Forward, HIGH);
  digitalWrite(motor2Backward, LOW);
  delay(300); // Adjust delay to control turn angle
}

void moveRight() {
  digitalWrite(motor1Forward, HIGH);
  digitalWrite(motor1Backward, LOW);
  digitalWrite(motor2Forward, LOW);
  digitalWrite(motor2Backward, HIGH);
  delay(300); // Adjust delay to control turn angle
}
