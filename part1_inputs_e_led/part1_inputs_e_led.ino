#include <Arduino.h>
#include <driver/gpio.h>
#include <Bounce2.h>
#include <hid.h>


#define MAX_BUTTONS 19
const gpio_num_t led_pin = GPIO_NUM_2;

const int BUTTON_PINS[MAX_BUTTONS] = { 3, 4, 5, 12, 13, 14, 15, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33};

Bounce* buttons[MAX_BUTTONS];

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.print("PIN_LED: ");
  Serial.println(String(led_pin));

  gpio_config_t io_conf = {
    .mode = GPIO_MODE_INPUT,
    .pull_up_en = GPIO_PULLUP_ENABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
  };

  for (int i = 0; i < MAX_BUTTONS; i++) {
    io_conf.pin_bit_mask = (1ULL << BUTTON_PINS[i]);
    gpio_config(&io_conf);
    buttons[i] = new Bounce(BUTTON_PINS[i], 2); // debounce interval of 2ms
  }


  gpio_set_direction(led_pin, GPIO_MODE_OUTPUT);

}

void loop() {
  for (int i = 0; i < MAX_BUTTONS; i++) {
    buttons[i]->update();
    if (buttons[i]->fell()) { // when button is pressed
      Serial.println(String(BUTTON_PINS[i]) + " apertado!");
      gpio_set_level(led_pin, 1); // turn on built-in LED
    }
    if (buttons[i]->rose()) { // when button is released
      gpio_set_level(led_pin, 0); // turn off built-in LED
    }
  }
  delay(1); // a small delay to avoid excessive processing
}