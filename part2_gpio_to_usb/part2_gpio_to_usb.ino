#include "driver/uart.h"
#include "driver/gpio.h"
#define ECHO_TEST_TXD (GPIO_NUM_4)
#define ECHO_TEST_RXD (GPIO_NUM_5)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)
#define BUF_SIZE (256)
const int inputPins[] = { 3, 4, 5, 12, 13, 14, 15, 18, 19, 21, 22, 23, 25, 26, 27, 32, 33 };
const int numInputPins = sizeof(inputPins) / sizeof(inputPins[0]);
const int ledPin = LED_BUILTIN;
const uart_port_t uart_num = UART_NUM_0;

void setup() {
  //const int uart_num = UART_NUM_0;
  const uart_port_t uart_num = UART_NUM_0;
  uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_param_config(uart_num, &uart_config);
  uart_set_pin(uart_num, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
  esp_err_t ret = uart_driver_install(uart_num, BUF_SIZE, 0, 0, NULL, 0);

  if (ret != ESP_OK) {
    Serial.println("Error installing UART driver");
    while (1) { delay(100); }
  }

  for (int i = 0; i < numInputPins; i++) {
    pinMode(inputPins[i], INPUT);
  }

  for (int i = 0; i < numInputPins; i++) {
    pinMode(inputPins[i], OUTPUT);
    digitalWrite(inputPins[i], HIGH);
  }

  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
}

uint8_t* data = (uint8_t*) malloc(BUF_SIZE);
bool buttonTriggered[numInputPins] = {false};
bool ledState = false;

void loop() {
    if(data == NULL) {
        Serial.println("Error allocating memory for data buffer");
        return;
    }
    bool inputTriggered = false;
    for (int i = 0; i < numInputPins; i++) {
        int pinValue = digitalRead(inputPins[i]);
        if (pinValue == LOW) {
            inputTriggered = true;
            if(!buttonTriggered[i]) {
                buttonTriggered[i] = true;
                ledState = !ledState;
                Serial.print("Pin ");
                Serial.print(inputPins[i]);
                Serial.print(": ");
                Serial.println(pinValue);
                Serial.print("Input triggered on pin: ");
                Serial.println(inputPins[i]);
            }
        } else {
            buttonTriggered[i] = false;
        }
    }
   digitalWrite(ledPin,inputTriggered ? HIGH : LOW);
    int len = uart_read_bytes(uart_num, data, BUF_SIZE, 20 / portTICK_RATE_MS);
    if (len > 0) {
        uart_write_bytes(uart_num, (const char*) data, len);
    }
    delay(10);
}
void cleanup() {
    free(data);
}