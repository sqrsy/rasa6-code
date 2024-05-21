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
int pot1_mV = 0;
int pot2_mV = 0;
int beats_per_cycle_1 = 1;
int beats_per_cycle_2 = 1;

// set up program logic
bool clk_reset = false;
int beat_counter = 0;
int millis_per_cycle = 5000;
int millis_residue = 0;
int millis_track = 0;
int millis_new = 0;
int millis_old = 0;
int millis_ref = 0;
int subdiv_length_1 = 0;
int subdiv_residue_1 = 0;
int subdiv_length_2 = 0;
int subdiv_residue_2 = 0;

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

  // convert first pot voltage into integer range 1-8
  pot1_mV = read_analog_mV(PIN_POT1, 0, 0);
  beats_per_cycle_1 = 8 - mV_to_integer(pot1_mV, 7, 1, 5);
  if(debug){
    Serial.print ("Pot 1 value: ");
    Serial.println (beats_per_cycle_1);
  }

  // convert second pot voltage into integer range 1-8
  pot2_mV = read_analog_mV(PIN_POT2, 0, 0);
  beats_per_cycle_2 = 8 - mV_to_integer(pot2_mV, 7, 1, 5);
  if(debug){
    Serial.print ("Pot 2 value: ");
    Serial.println (beats_per_cycle_2);
  }

  //////////////////////////////////////////////////////////////////
  ///// RUN PROGRAM LOGIC
  //////////////////////////////////////////////////////////////////

  // only run once per clock cycle
  // run on rising edge of incoming clock signal
  millis_new = millis();
  if(current_clk & clk_reset){

    // figure out how long it takes to complete one clock cycle
    //  (from rising edge to rising edge)
    millis_per_cycle = millis_new - millis_old;
    millis_old = millis_new;
    millis_ref = millis_new;

    if(debug) Serial.println ("Advance clock.");
    clk_reset = false;
  }

  // only run once per clock cycle
  // run on falling edge of incoming clock signal
  if(!current_clk & !clk_reset){
    clk_reset = true;
  }
  
  millis_track = millis_new - millis_ref; // time since last clock signal
  millis_residue = millis_track % millis_per_cycle; // how far along in the clock signal
  subdiv_length_1 = millis_per_cycle / beats_per_cycle_1; // length of 1 sub-division (chan A) 
  subdiv_length_2 = millis_per_cycle / beats_per_cycle_2; // length of 1 sub-division (chan B)
  subdiv_residue_1 = millis_residue % subdiv_length_1; // how far along in the sub-division (chan A)
  subdiv_residue_2 = millis_residue % subdiv_length_2; // how far along in the sub-division (chan B)
  if(debug){
    Serial.print ("Length of clock cycle (ms): ");
    Serial.println (millis_per_cycle);
    Serial.print ("Time since last clock signal (ms): ");
    Serial.println (millis_residue);
    Serial.print ("Sub-division length 1 value (ms): ");
    Serial.println (subdiv_length_1);
    Serial.print ("Sub-division length 2 value (ms): ");
    Serial.println (subdiv_length_2);
    Serial.print ("Sub-division residue 1 value (ms): ");
    Serial.println (subdiv_residue_1);
    Serial.print ("Sub-division residue 2 value (ms): ");
    Serial.println (subdiv_residue_2);
  }

  //////////////////////////////////////////////////////////////////
  ///// WRITE OUTPUT
  //////////////////////////////////////////////////////////////////

  if(subdiv_residue_1 < (subdiv_length_1 / 2)){
    digitalWrite(PIN_OUT1, 1); // PWM at 50%
  }else{
    digitalWrite(PIN_OUT1, 0);
  }

  if(subdiv_residue_2 < (subdiv_length_2 / 2)){
    digitalWrite(PIN_OUT2, 1); // PWM at 50%
  }else{
    digitalWrite(PIN_OUT2, 0);
  }
  
  if(debug) delay(250);
}
