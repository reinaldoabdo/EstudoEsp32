#include <BleGamepad.h>

BleGamepad bleGamepad("KRArcade Negative Delay", "ReinaldoAbdo", 100);
BleGamepadConfiguration bleGamepadConfig; 

// Pinos usados para o D-pad
const int dpadUpPin = 32;
const int dpadDownPin = 33;
const int dpadLeftPin = 25;
const int dpadRightPin = 26;

// Pinos usados para os botões
const int buttonPins[] = { 3, 4, 5, 12, 13, 14, 15, 18, 19, 21, 22 };

void setup() {
  // Configura os pinos do D-pad como entradas com pull-up interno
  pinMode(dpadUpPin, INPUT_PULLUP);
  pinMode(dpadDownPin, INPUT_PULLUP);
  pinMode(dpadLeftPin, INPUT_PULLUP);
  pinMode(dpadRightPin, INPUT_PULLUP);

  // Configura os pinos dos botões como entradas com pull-up interno
  for (int i = 0; i < sizeof(buttonPins) / sizeof(int); i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Inicializa o gamepad com a configuração especificada
  bleGamepad.begin(&bleGamepadConfig);
}

void loop() {
  if (bleGamepad.isConnected()) {
    // Lê o estado dos pinos do D-pad e atualiza o gamepad
    if (digitalRead(dpadUpPin) == LOW) {
      bleGamepad.setHat1(HAT_UP);
    } else if (digitalRead(dpadDownPin) == LOW) {
      bleGamepad.setHat1(HAT_DOWN);
    } else if (digitalRead(dpadLeftPin) == LOW) {
      bleGamepad.setHat1(HAT_LEFT);
    } else if (digitalRead(dpadRightPin) == LOW) {
      bleGamepad.setHat1(HAT_RIGHT);
    } else {
      bleGamepad.setHat1(HAT_CENTERED);
    }

    // Lê o estado dos pinos dos botões e atualiza o gamepad
    for (int i = 0; i < sizeof(buttonPins) / sizeof(int); i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        bleGamepad.press(i + 1);
      } else {
        bleGamepad.release(i + 1);
      }
    }
  }

  delay(10);
}