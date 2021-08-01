// Offer DIY Workshop
#include "src/PD_UFP.h"
#include <EEPROM.h>
// To log raw packet, init PD_UFP with parameter PD_UFP(PD_UFP(PD_LOG_LEVEL_VERBOSE)
class PD_UFP_log_c PD_UFP;
char pd_out[] = {5, 9, 12, 15, 20};
int address = 0;
unsigned char value = 0 ;
const int buttonPin = 9;     // the number of setup pin
int buttonState = 0;
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
  // PD_UFP.init(value);
  EEPROM.update(address, value);
}
void loop() {
  PD_UFP.run();
  PD_UFP.print_status(Serial1);

  if (PD_UFP.is_power_ready()) {
    if (PD_UFP.get_voltage() == PD_V(pd_out[value]) && PD_UFP.get_current() >= PD_A(1.0)) {
      //    if (PD_UFP.get_voltage() == PD_V(5) && PD_UFP.get_current() >= PD_A(1.0)) {
      PD_UFP.set_output(1);
      PD_UFP.set_led(1);
    } else {
      PD_UFP.set_output(0);
      PD_UFP.blink_led(400);
    }
  }
}
