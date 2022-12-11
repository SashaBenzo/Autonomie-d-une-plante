// Pin definitions
const int motorPin = Pin # ; /////////////// Motor is connected to digital pin
const int sensorPin = AnalogPin# ; ///// Moisture sensor is connected to analog pin

// Motor on/off threshold value
const int threshold = {insert val} ;

void setup() {
  // Set the motor pin as an output
  pinMode(motorPin, OUTPUT);
}

void loop() {
  // Read the moisture sensor value
  int sensorValue = analogRead(sensorPin);

  // If the moisture value is below the threshold, turn on the motor
  if (sensorValue < threshold) {
    digitalWrite(motorPin, HIGH);
  } else {
    digitalWrite(motorPin, LOW);
  }
}
