/*
VCC to arduino 5v GND to arduino GND
Echo to Arduino pin 12 Trig to Arduino pin 11
Led to pin 13
Buzzer to plus to pin 9
Buzzer minus to ground
*/

#define trigPin 11
#define echoPin 12
#define led 13
#define speakerPin 9

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode (speakerPin, OUTPUT);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
//  delayMicroseconds(1000); - Removed this line
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  if (distance < 40) {  // This is where the LED On/Off happens
    digitalWrite(led,HIGH); // When the Red condition is met, the Green LED should turn off
    tone(speakerPin, 10);
    delay (500);
    noTone(speakerPin);
    delay(10);
}
  else {
    digitalWrite(led,LOW);
  }

  delay(500);
//   digitalWrite(led,HIGH); // When the Red condition is met, the Green LED should turn off
}
