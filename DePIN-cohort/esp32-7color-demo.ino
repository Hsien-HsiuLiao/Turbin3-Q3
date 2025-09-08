int vDelay = 500;  //Controls blink speed of built in and RGB leds

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void toggleLED() {
  if (digitalRead(LED_BUILTIN) == LOW){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
   digitalWrite(LED_BUILTIN, LOW);
  }
}

void loop() {
  //1
  toggleLED();
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);

  delay(vDelay);
//2
 toggleLED();
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, HIGH);

  delay(vDelay);
  //3
  toggleLED();
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);

  delay(vDelay);
  //4
 toggleLED();
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);

  delay(vDelay);
  //5
  toggleLED();
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, LOW);

  delay(vDelay);
  //6
 toggleLED();
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, HIGH);

   delay(vDelay);
   //7
  toggleLED();
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);

  delay(vDelay);
  //8  Turns RGB LED completely Off
  /*
 toggleLED();
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE, HIGH);
  
  delay(vDelay);
 */
  //toggleLED();
}