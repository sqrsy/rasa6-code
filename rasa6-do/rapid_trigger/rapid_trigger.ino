// for beetle, setup with Leonardo board
#include "digitalWrite_at_fixed_interval.h"
#include "eurotools-v2.h"

// set up pin config
int PIN_CLKIN = A2;
int PIN_POT1 = A0;
int PIN_POT2 = A1;
int PIN_OUT1 = 10;
int PIN_OUT2 = 11;

// set up hardware constants
int R1_VALUE = 220;
int R2_VALUE = 150;
bool REVERSE_POT = true;

// set up to read input
bool current_clk = false;
int pot1_mV = 0;
int pot2_mV = 0;
int number_of_triggers = 0;
int trigger_duration_ms = 0;

// set up program logic
bool clk_reset = false;
bool cycle_finished = false;
int triggers_available = 0;

// toggle debug
bool debug = false;

void setup() {

  // set up all PINS for the chip
  if (debug) Serial.begin(9600);
  pinMode(PIN_CLKIN, INPUT);
  pinMode(PIN_POT1, INPUT);
  pinMode(PIN_POT2, INPUT);
  pinMode(PIN_OUT1, OUTPUT);
  pinMode(PIN_OUT2, OUTPUT);
}

void loop() {

  //////////////////////////////////////////////////////////////////
  ///// READ INPUT
  //////////////////////////////////////////////////////////////////

  // set incoming mV signal to TRUE if more than 500 mV
  current_clk = read_analog_bool(PIN_CLKIN, 500, R1_VALUE, R2_VALUE, debug);

  // convert first pot voltage into integer range 0-7 => map to 1-8
  pot1_mV = read_analog_mV(PIN_POT1, 0, 0);
  number_of_triggers = mV_to_integer(pot1_mV, 7, 1, 5);  // integer ranges 0-7
  number_of_triggers = 8 - number_of_triggers;
  if (debug) {
    Serial.print("Pot 1 value: ");
    Serial.println(number_of_triggers);
  }

  // convert second pot voltage into integer range 0-7 => map to 10-80
  pot2_mV = read_analog_mV(PIN_POT2, 0, 0);
  trigger_duration_ms = mV_to_integer(pot2_mV, 7, 1, 5);  // integer ranges 0-7
  trigger_duration_ms = 10 * (8 - trigger_duration_ms);
  if (debug) {
    Serial.print("Pot 2 value: ");
    Serial.println(trigger_duration_ms);
  }

  //////////////////////////////////////////////////////////////////
  ///// RUN PROGRAM LOGIC
  //////////////////////////////////////////////////////////////////

  // only run once per clock cycle
  // run on rising edge of incoming clock signal
  if (current_clk & clk_reset) {

    // reset the rapid fire counter
    // there are Y rapid fire triggers per event
    triggers_available = number_of_triggers;

    if (debug) Serial.println("Advance clock.");
    clk_reset = false;
  }

  // only run once per clock cycle
  // run on falling edge of incoming clock signal
  if (!current_clk & !clk_reset) {
    clk_reset = true;
  }

  //////////////////////////////////////////////////////////////////
  ///// WRITE OUTPUT
  //////////////////////////////////////////////////////////////////

  // when triggered by clock rise:
  // - write and decrement triggers until none are available in pool
  // - each trigger writes X/2 ms HIGH followed by X/2 ms LOW
  // - cycle_finished = true after X ms has passed
  if (triggers_available > -1) {  // one trigger is spent to set up the cycle_finished switch

    digitalWrite(PIN_OUT2, 1);  // the second output sends TRUE while rapid fire trigger occurs
    cycle_finished = digitalWrite_at_fixed_interval(PIN_OUT1, trigger_duration_ms);
    if (cycle_finished) {
      triggers_available = triggers_available - 1;
      if (debug) {
        Serial.print("Triggers available: ");
        Serial.println(triggers_available);
      }
    }

  } else {

    // disable all output
    digitalWrite(PIN_OUT1, 0);
    digitalWrite(PIN_OUT2, 0);
  }

  if (debug) delay(250);
}
