const int numLEDs = 11;  // Number of LEDs
int ledPins[numLEDs] = {13,12,14,27,26,25,32,33,15,2,4};  // Define the pin numbers for your LEDs

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], HIGH);  // Turn on all LEDs
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
