#include <Servo.h>

// Define the servo objects
Servo servo4;
Servo servo5;
Servo servo6;
Servo servo7;

// Define initial and final positions
int initialPositionM = 60;
int initialPositionL = 40;
int initialPosition4; // Dynamically assigned based on sensor readings
int finalPosition4 = 135;

int initialPosition5 = 60;
int midPosition5 = 25;
int finalPosition5 = 0;

int initialPosition6 = 0;
int finalPosition6 = 60;

int initialPosition7 = 80;
int finalPosition7 = 160;

// Define the ultrasonic sensor pins
const int trigPin = 11;
const int echoPin = 12;

long duration;
int distance;
unsigned long previousMillis = 0;
const long interval = 100; // Read ultrasonic sensor every 100 ms (adjust as needed)

// Function declarations
void moveToDefaultPosition();
void moveSequence();

void setup() {
  // Attach the servos to their respective pins
  servo4.attach(4);
  servo5.attach(5);
  servo6.attach(6);
  servo7.attach(7);

  // Set the ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Start serial communication
  Serial.begin(9600);

  // Set the default initial position for servo 4
  initialPosition4 = 90; // Neutral position when no object is detected

  // Move servos to their default (neutral) positions
  moveToDefaultPosition();
}

void loop() {
  // Check the time since the last distance measurement
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Update the time

    // Measure the distance from the ultrasonic sensor
    distance = getDistance();

    // Print the distance to the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Check distance and determine if the robot should move
    if (distance == 6 || distance == 7) {
      initialPosition4 = initialPositionL;
      moveSequence();  // Execute the movement sequence
    } else if (distance == 8 || distance == 9) {
      initialPosition4 = initialPositionM;
      moveSequence();  // Execute the movement sequence
    } else {
      // If the distance is not within the range, move to default position
      moveToDefaultPosition();
    }
  }
}

void moveToDefaultPosition() {
  // Move all servos to their default (neutral) positions
  servo4.write(90);  // Default neutral position for servo 4
  servo5.write(initialPosition5);
  servo6.write(initialPosition6);
  servo7.write(initialPosition7);
}

void moveSequence() {
  // Step 1: Move servo 4 from the dynamic initial position to 135
  moveServo(servo4, initialPosition4, finalPosition4);

  // Step 2: Move servo 5 from 60 to 0
  moveServo(servo5, initialPosition5, finalPosition5);

  // Step 3: Move servo 6 from 0 to 60
  moveServo(servo6, initialPosition6, finalPosition6);

  // Step 4: Move servo 7 from 80 to 160
  moveServo(servo7, initialPosition7, finalPosition7);

  // Step 5: Move servo 6 from 60 to 0
  moveServo(servo6, finalPosition6, initialPosition6);

  // Step 6: Move servo 5 from 0 to 60
  moveServo(servo5, finalPosition5, initialPosition5);

  // Step 7: Move servo 4 from 135 back to the dynamic initial position
  moveServo(servo4, finalPosition4, initialPosition4);

  // Step 8: Move servo 6 from 0 to 60
  moveServo(servo6, initialPosition6, finalPosition6);

  // Step 9: Move servo 5 from 60 to 25
  moveServo(servo5, initialPosition5, midPosition5);

  // Step 10: Move servo 7 from 160 to 80
  moveServo(servo7, finalPosition7, initialPosition7);
}

void moveServo(Servo &servo, int start, int end) {
  if (start < end) {
    for (int pos = start; pos <= end; pos++) {
      servo.write(pos);
      delay(5); // Reduced delay for faster servo movement
    }
  } else {
    for (int pos = start; pos >= end; pos--) {
      servo.write(pos);
      delay(5); // Reduced delay for faster servo movement
    }
  }
}

// Function to get distance from ultrasonic sensor
int getDistance() {
  // Clear the trigPin by setting it LOW
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Trigger the sensor by setting the trigPin HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin and calculate the distance
  duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (round trip)

  return distance;
}
