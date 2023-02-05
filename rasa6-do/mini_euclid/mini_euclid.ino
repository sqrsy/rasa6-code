// for beetle, setup with Leonardo board
#include "eurotools-v2.h"
#include "update_euclid_rule.h"

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
int events_per_cycle = 0;
int beats_per_cycle = 0;

// set up program logic
bool clk_reset = false;
int beat_counter = 0;
int event_swap = 0;
int euclid_rule1[32];
int offset = 0;

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
  events_per_cycle = 8 - mV_to_integer(pot1_mV, 7, 1, 5);
  if(debug){
    Serial.print ("Pot 1 value: ");
    Serial.println (events_per_cycle);
  }

  // convert second pot voltage into integer range 1-8
  pot2_mV = read_analog_mV(PIN_POT2, 0, 0);
  beats_per_cycle = 8 - mV_to_integer(pot2_mV, 7, 1, 5);
  if(debug){
    Serial.print ("Pot 2 value: ");
    Serial.println (beats_per_cycle);
  }
  
  // handle when there are more events than beats
  if(events_per_cycle > beats_per_cycle){
    event_swap = events_per_cycle;
    events_per_cycle = beats_per_cycle;
    beats_per_cycle = event_swap;
    if(debug){
      Serial.print ("Pot 1 replacement value: ");
      Serial.println (events_per_cycle);
    }
  }
  
  //////////////////////////////////////////////////////////////////
  ///// RUN PROGRAM LOGIC
  //////////////////////////////////////////////////////////////////
  
  // only run once per clock cycle
  // run on rising edge of incoming clock signal
  if(current_clk & clk_reset){

    // update the beat counter
    beat_counter = beat_counter + 1;
    beat_counter = beat_counter % beats_per_cycle;

    if(debug) Serial.println ("Advance clock.");
    clk_reset = false;
  }

  // only run once per clock cycle
  // run on falling edge of incoming clock signal
  if(!current_clk & !clk_reset){
    clk_reset = true;
  }

  // update Euclid rule at start of cycle
  if(beat_counter == 0){
    update_euclid_rule(euclid_rule1, events_per_cycle, beats_per_cycle, offset);
    if(debug){
      Serial.print("Euclid rule: ");
      for(int z = 0; z < beats_per_cycle; z = z + 1){
        Serial.print (euclid_rule1[z]);
      }
      Serial.println ("");
    }
  }

  //////////////////////////////////////////////////////////////////
  ///// WRITE OUTPUT
  //////////////////////////////////////////////////////////////////

  // the Euclid Rule describes whether beat has a trigger
  // when clock rises HIGH, write output based on Euclid Rule
  // when clock falls LOW, disable output
  if(current_clk){
    digitalWrite(PIN_OUT1, euclid_rule1[beat_counter]); // outputs Euclid Rule
    digitalWrite(PIN_OUT2, !euclid_rule1[beat_counter]); // outputs NOT(Euclid Rule)
  }else{
    digitalWrite(PIN_OUT1, 0);
    digitalWrite(PIN_OUT2, 0);
  }

  if(debug) delay(250);
}
