#include <Arduino.h>

int val;
int counter = 0; // Initialize a counter variable

void setup() {
  // put your setup code here, to run once:
 // delay(50000);
 Serial.begin(115200);
  //SerialUSB.begin(); // Only if using native USB

  delay(1000);
  Serial.println("Hello UART!");
  //SerialUSB.println("Hello USB!");

     Serial.println("Enter capital E");

}

void loop() {
  // put your main code here, to run repeatedly:
 val = Serial.read();
  if (val == 'E')
  {
    delay(500);
    delay(500);
    Serial.println("Hello from vbox pio esp32! it works sept 28");
        Serial.println(counter);

    counter++;
  }
}