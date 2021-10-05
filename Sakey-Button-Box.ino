// For our configuration
#include <Keyboard.h> // Native USB Keyboard libs
#include "non_ascii_keys.h" // Included file defining non-standard keys
#include "config.h" // Our button binding configs

// For the IO expanders
#include <Wire.h>        // Native I2C libs
#include <MCP23017.h>    // "MCP23017" by Bertrand Lemalse
MCP23017 MCP0 = MCP23017(0x20);
MCP23017 MCP1 = MCP23017(0x21);
uint64_t oldButtonStates;
uint64_t newButtonStates;

// For the LED PWM controller
#include <Adafruit_TLC5947.h> // "Adafruit TLC5947" by Adafruit
#define NUM_TLC5974    1
#define SPI_DATA_PIN  16
#define SPI_CLOCK_PIN 15
#define SPI_CS_PIN    10
Adafruit_TLC5947 tlc = Adafruit_TLC5947(NUM_TLC5974, SPI_CLOCK_PIN, SPI_DATA_PIN, SPI_CS_PIN);

// For misc buttons
#define BUTTON_5  A1
#define BUTTON_10 A0
#define DPAD_DEBOUNCE_TIME_MILLIS 200

// Timers
unsigned long currentMillis           = 0;
unsigned long updateOutputMillis      = 0;
unsigned long button5DebounceMillies  = 0;
unsigned long button10DebounceMillies = 0;

// Function to control the LEDs
void setLED(int LED, int brightness) {
  if ((LED < 0) || (LED > 23)) { return; } // Ignore invalid LEDs
  if (LED >= 12) { tlc.setPWM(LED, 40 * brightness); }
  else { tlc.setPWM(LED, 5 * brightness); }
  tlc.write();
}

// Helper functions for bitRead/bitWrite that supports 64-bit numbers
void bitWrite64(uint64_t &number, uint8_t bitPos, uint64_t newBit) {
  number ^= (-newBit ^ number) & (1ULL << bitPos);
}
bool bitRead64(uint64_t number, uint8_t bitPos) {
  return (bool)((number >> bitPos) & 1ULL);
}

// Function to control our outputs
void updateOutputs() {
  // First we check our D-Pad centre buttons 5 and 10 are pressed
  if (bitRead64(newButtonStates, 32) == LOW) { button5DebounceMillies = currentMillis; }
  if (bitRead64(newButtonStates, 33) == LOW) { button10DebounceMillies = currentMillis; }

  // If they are pressed, cancel out their respective surrounding buttons
  if (currentMillis - button5DebounceMillies < DPAD_DEBOUNCE_TIME_MILLIS) {
    for (int i = 24; i <= 27; i++) { bitWrite64(oldButtonStates, i, HIGH); bitWrite64(newButtonStates, i, HIGH); }
  }
  if (currentMillis - button10DebounceMillies < DPAD_DEBOUNCE_TIME_MILLIS) {
    for (int i = 28; i <= 31; i++) { bitWrite64(oldButtonStates, i, HIGH); bitWrite64(newButtonStates, i, HIGH); }
  }

  if (DEBUG) {
    Serial.print("New button states: ");
    for (int i = 0; i < 34; i++) {
      int j = bitRead64(newButtonStates, i);
      Serial.print(j);
      if (i % 6 == 5) { Serial.print(" "); }
    }
    Serial.println();
    Serial.print("Old button states: ");
    for (int i = 0; i < 34; i++) {
      int j = bitRead64(oldButtonStates, i);
      Serial.print(j);
      if (i % 6 == 5) { Serial.print(" "); }
    }
    Serial.println();
  }

  // Loop over each button to see if it's changed
  for (int i = 0; i < 34; i++) {
    bool buttonState = bitRead64(newButtonStates, i);

    if (buttonState != bitRead64(oldButtonStates, i)) { // If this button has actually changed state
      switch (keybindings[i][4]) {
        case BUTTON_TAP_ON_RELEASE: // For any button set to tap once when released
          bitWrite64(oldButtonStates, i, bitRead64(newButtonStates, i)); // Update our knowledge of this button state
          if (buttonState == HIGH) { // If button has been released
            for (int j = 0; j <= 3; j++) {
              if (keybindings[i][j] != KEY_NONE) {
                if (DEBUG) { Serial.print("Sending key: "); Serial.println(keybindings[i][j]); }
                Keyboard.press(keybindings[i][j]);
              }
            }
            setLED(i, LED_ACTIVE_BRIGHTNESS);
            delay(25);
            for (int j = 3; j >= 0; j--) {
              if (keybindings[i][j] != KEY_NONE) {
                if (DEBUG) { Serial.print("Release key: "); Serial.println(keybindings[i][j]); }
                Keyboard.release(keybindings[i][j]);
              }
            }
            setLED(i, LED_IDLE_BRIGHTNESS);
          }
          break;
        case BUTTON_RAPID_FIRE: // For any button set to rapid fire
          if (buttonState == LOW) { // If button has been pressed
            for (int j = 0; j <= 3; j++) {
              if (keybindings[i][j] != KEY_NONE) {
                if (DEBUG) { Serial.print("Sending key: "); Serial.println(keybindings[i][j]); }
                Keyboard.press(keybindings[i][j]);
              }
            }
            setLED(i, LED_ACTIVE_BRIGHTNESS);
            delay(25);
            for (int j = 3; j >= 0; j--) {
              if (keybindings[i][j] != KEY_NONE) {
                if (DEBUG) { Serial.print("Release key: "); Serial.println(keybindings[i][j]); }
                Keyboard.release(keybindings[i][j]);
              }
            }
            setLED(i, LED_IDLE_BRIGHTNESS);
            delay(25);
          }
          break;
        default: // For all other button presses, treat like a regular keyboard switch
          bitWrite64(oldButtonStates, i, bitRead64(newButtonStates, i)); // Update our knowledge of this button state
          if (buttonState == LOW) { // If button has been pressed
            for (int j = 0; j <= 3; j++) {
              if (keybindings[i][j] != KEY_NONE) {
                if (DEBUG) { Serial.print("Sending key: "); Serial.println(keybindings[i][j]); }
                Keyboard.press(keybindings[i][j]);
              }
            }
            setLED(i, LED_ACTIVE_BRIGHTNESS);
          } else { // Button has been released
            for (int j = 3; j >= 0; j--) {
              if (keybindings[i][j] != KEY_NONE) {
                if (DEBUG) { Serial.print("Release key: "); Serial.println(keybindings[i][j]); }
                Keyboard.release(keybindings[i][j]);
              }
            }
            setLED(i, LED_IDLE_BRIGHTNESS);
          }
          break;
      }
    }
  }
}

// Function to initialise the IO expander
void initIOExpanders() {
  // Initialise I2C
  Wire.begin();
  Wire.setClock(400000UL);

  // Init first IO expander
  MCP0.init();
  MCP0.portMode(MCP23017Port::A, 0xFF);
  MCP0.portMode(MCP23017Port::B, 0xFF);
  MCP0.writeRegister(MCP23017Register::GPIO_A, 0x00);
  MCP0.writeRegister(MCP23017Register::GPIO_B, 0x00);
  MCP0.writeRegister(MCP23017Register::IPOL_A, 0x00);
  MCP0.writeRegister(MCP23017Register::IPOL_B, 0x00);

  // Init second IO expander
  MCP1.init();
  MCP1.portMode(MCP23017Port::A, 0xFF);
  MCP1.portMode(MCP23017Port::B, 0xFF);
  MCP1.writeRegister(MCP23017Register::GPIO_A, 0x00);
  MCP1.writeRegister(MCP23017Register::GPIO_B, 0x00);
  MCP1.writeRegister(MCP23017Register::IPOL_A, 0x00);
  MCP1.writeRegister(MCP23017Register::IPOL_B, 0x00);
}

void setup() {
  // Turn off serial if debug mode isn't enabled to speed up response time
  if (DEBUG) {
    Serial.begin(115200);
    delay(2000); // To allow serial monitor time to reconnect
    Serial.println("Initialising IO expanders...");
  }
  initIOExpanders();
  if (DEBUG) { Serial.println("Initialising LED controller..."); }
  tlc.begin();
  for (int i = 0; i <= 23; i++) { setLED(i, LED_IDLE_BRIGHTNESS); delay(25); }
  delay(500);
  for (int i = 23; i >= 0; i--) { setLED(i, 0); delay(25); }
  delay(500);
  if (DEBUG) { Serial.println("Initialising A0 and A1 as additional inputs..."); }
  pinMode(BUTTON_5, INPUT_PULLUP);
  pinMode(BUTTON_10, INPUT_PULLUP);
  if (DEBUG) { Serial.println("Initialising keyboard mode..."); }
  for (int i = 0; i <= 33; i++) { bitWrite64(oldButtonStates, i, HIGH); setLED(i, LED_IDLE_BRIGHTNESS); }
  Keyboard.begin();
  if (DEBUG) { Serial.println("Initialisation complete. Game on!"); }
}

void loop() {
  // Get our current loop timestamp
  currentMillis = millis();

  // Read IO from IO expanders
  newButtonStates = (uint64_t)((uint64_t)MCP1.read() << 16 | MCP0.read());

  // Read hat switch buttons 5 and 10 respectively
  newButtonStates |= (uint64_t)((uint64_t)digitalRead(BUTTON_5) << 32);
  newButtonStates |= (uint64_t)((uint64_t)digitalRead(BUTTON_10) << 33);

  // Check if IO pins have changed, if they have send an immediate change to the PC
  if (newButtonStates != oldButtonStates) { updateOutputs(); updateOutputMillis = currentMillis; }
    
  // Timed activities
  if (currentMillis - updateOutputMillis > 1000) { updateOutputMillis = currentMillis; updateOutputs(); } // Update by default every 100ms
}
