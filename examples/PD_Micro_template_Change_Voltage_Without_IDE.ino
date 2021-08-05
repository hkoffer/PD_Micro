#include "src/PD_UFP.h"
#include <EEPROM.h>
class PD_UFP_log_c PD_UFP;
char pd_out[] = {5, 9, 12, 15, 20};
int address = 0;
unsigned char value = 0 ;
const int buttonPin = 9;     // the IO number of setup pin
int buttonState = 0;
/*
    PD_POWER_OPTION_MAX_5V      = 0,
    PD_POWER_OPTION_MAX_9V      = 1,
    PD_POWER_OPTION_MAX_12V     = 2,
    PD_POWER_OPTION_MAX_15V     = 3,
    PD_POWER_OPTION_MAX_20V     = 4,
*/
void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  value = EEPROM.read(address);
  Serial1.begin(115200);  // Serial1 is hardware serial on Pin D0 and D1
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    value++;
    if (value > 4) {
      value = 0;
    }
  }
  PD_UFP.init(value);
  EEPROM.update(address, value);
}
void loop() {
  PD_UFP.run();
  PD_UFP.print_status(Serial1);

  if (PD_UFP.is_power_ready()) {
    if (PD_UFP.get_voltage() == PD_V(pd_out[value]) && PD_UFP.get_current() >= PD_A(1.0)) {
      PD_UFP.set_output(1);
      PD_UFP.set_led(1);
    } else {
      PD_UFP.set_output(0);
      PD_UFP.blink_led(400);
    }
  }
}
