// for beetle, setup with Leonardo board
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
int pot1_pct = 0;
int pot2_pct = 0;

// set up program logic
bool clk_reset = false;
int randA = 0;
bool sendA = false;
int randB = 0;
bool sendB = false;

// toggle debug
bool debug = false;

void setup() {
  
  // set up all PINS for the chip
  if(debug) Serial.begin(9600);
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

  // read pot voltages as percent
  pot1_pct = read_analog_pct(PIN_POT1, 5000, REVERSE_POT, 0, 0, debug);
  pot2_pct = read_analog_pct(PIN_POT2, 5000, REVERSE_POT, 0, 0, debug);

  //////////////////////////////////////////////////////////////////
  ///// RUN PROGRAM LOGIC
  //////////////////////////////////////////////////////////////////

  // only run once per clock cycle
  // run on rising edge of incoming clock signal
  if(current_clk & clk_reset){

    // run probability check
    // note: p(B) is conditional on p(A)
    // send A/B if knob is bigger than runif
    // if knob is 0, it never sends
    // if knob is 100, it always sends
    randA = random(1, 100);
    randB = random(1, 100);
    sendA = false; // must reset every time!
    sendB = false;
    if(pot1_pct >= randA){
      sendA = true;
    }else{
       if(pot2_pct >= randB){
        sendB = true;
      }
    }

    if(debug){
      Serial.print ("Status A: ");
      Serial.println (randA);
      Serial.println (sendA);
      Serial.print ("Status B: ");
      Serial.println (randB);
      Serial.println (sendB);
    }
    
    clk_reset = false;
  }

  // only run once per clock cycle
  // run on falling edge of incoming clock signal
  if(!current_clk & !clk_reset){
    clk_reset = true;
  }

  //////////////////////////////////////////////////////////////////
  ///// WRITE OUTPUT
  //////////////////////////////////////////////////////////////////

  if(current_clk){
    digitalWrite(PIN_OUT1, sendA);
    digitalWrite(PIN_OUT2, sendB);
  }else{
    digitalWrite(PIN_OUT1, 0);
    digitalWrite(PIN_OUT2, 0);
  }

  if(debug) delay(250);
}
