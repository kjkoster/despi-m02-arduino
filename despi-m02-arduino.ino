//
// Sample code that shows the board features of the DESPI-M02. Well, all but
// the actual SPI interface, which is the whole point of this board. :-)
//
// We show how to blink `LED4` and how to control the blink time using button
// `S1`. Trace information is sent to the serial console, which we initialise
// using the correct pins for this board.
//

#define PIN_SERIAL_TX PA9   // on-board UART TX pin
#define PIN_SERIAL_RX PA10  // on-board UART RX pin
#define S1 PE11             // the on-board bush-button
#define LED_BUILTIN PE12    // the only controllable, on-board LED

volatile bool fast = false;
volatile unsigned long suppress_interrupts_until = millis();
const unsigned long bounce_time_ms = 500;

bool high = false;

void setup() {
  // The generic config does not define the TX and RX pins correctly.
  Serial.setTx(PIN_SERIAL_TX);
  Serial.setRx(PIN_SERIAL_RX);
  Serial.begin(57600);
  delay(500);
  Serial.println("booting...");

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(S1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(S1), on_S1_pressed, FALLING);
}

void on_S1_pressed() {
  const unsigned long now = millis();
  if (now > suppress_interrupts_until) {
    suppress_interrupts_until = now + bounce_time_ms;
    fast = !fast;

    Serial.println(fast ? "fast..." : "slow...");
  } else {
    // ignore this interrupt. Probably a bounce.
    Serial.println("bounce...");
  }
}

void loop() {
  Serial.printf("loop()...\n");

  high = !high;
  digitalWrite(LED_BUILTIN, high ? HIGH : LOW);
  delay(fast ? 100 : 500);
}
