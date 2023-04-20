#include <BleGamepad.h>

BleGamepad bleGamepad("KRArcade", "ReinaldoAbdo", 100);
BleGamepadConfiguration bleGamepadConfig;

const int DPAD_PINS[] = { 32, 33, 27, 14 };  //up, left, down, right
const int BUTTON_PINS[] = { 3, 4, 5, 12, 13, 15, 16, 17, 18, 19, 21, 22, 23 };
const int TRIGGER_PINS[] = { 25, 26 };
bool buttonPressed[sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0])] = { false };
bool dpadPressed[sizeof(DPAD_PINS) / sizeof(DPAD_PINS[0])] = { false };

void setup() {
  Serial.begin(115200);
  bleGamepad.begin();

  pinMode(LED_BUILTIN, OUTPUT);

  for (int i = 0; i < sizeof(DPAD_PINS) / sizeof(DPAD_PINS[0]); i++) {
    pinMode(DPAD_PINS[i], INPUT_PULLUP);
  }

  for (int i = 0; i < sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0]); i++) {
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }

  for (int i = 0; i < sizeof(TRIGGER_PINS) / sizeof(TRIGGER_PINS[0]); i++) {
    pinMode(TRIGGER_PINS[i], INPUT_PULLUP);
  }
}



void loop() {

  int8_t hatValue = 0;
  if (digitalRead(DPAD_PINS[0]) == LOW && digitalRead(DPAD_PINS[1]) == LOW) {
    hatValue = 8;  // up-left
    // Serial.println("up-left");
  } else if (digitalRead(DPAD_PINS[0]) == LOW && digitalRead(DPAD_PINS[3]) == LOW) {
    hatValue = 2;  // up-right
    // Serial.println("up-right");
  } else if (digitalRead(DPAD_PINS[2]) == LOW && digitalRead(DPAD_PINS[1]) == LOW) {
    hatValue = 6;  // down-left
    // Serial.println("down-left");
  } else if (digitalRead(DPAD_PINS[2]) == LOW && digitalRead(DPAD_PINS[3]) == LOW) {
    hatValue = 4;  // down-right
    // Serial.println("down-right");
  } else if (digitalRead(DPAD_PINS[0]) == LOW) {
    hatValue = 1;  // up
    // Serial.println("up");
  } else if (digitalRead(DPAD_PINS[1]) == LOW) {
    hatValue = 7;  // left
    // Serial.println("left");
  } else if (digitalRead(DPAD_PINS[2]) == LOW) {
    hatValue = 5;  // down
    // Serial.println("down");
  } else if (digitalRead(DPAD_PINS[3]) == LOW) {
    hatValue = 3;  // right
    // Serial.println("right");
  }

  bleGamepad.setHat(hatValue);

  int leftSliderValue = !digitalRead(TRIGGER_PINS[0]) ? 65535 : 0;
  int rightSliderValue = !digitalRead(TRIGGER_PINS[1]) ? 65535 : 0;

  Serial.println(leftSliderValue);
  Serial.println(rightSliderValue);
  
  bleGamepad.setAxes(0, 0, leftSliderValue, rightSliderValue, 0, 0, 0, 0);

  for (int i = 0; i < sizeof(BUTTON_PINS) / sizeof(BUTTON_PINS[0]); i++) {

    if (digitalRead(BUTTON_PINS[i]) == 0) {
      bleGamepad.press(i + 1);
      buttonPressed[i] = true;
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.print("Apertou: ");
      Serial.print(i);
      Serial.print("\n");
    } else {
      bleGamepad.release(i + 1);
    }
  }

  bleGamepad.sendReport();

  digitalWrite(LED_BUILTIN, LOW);

  delay(100);
}