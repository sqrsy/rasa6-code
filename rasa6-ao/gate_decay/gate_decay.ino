// for beetle, setup with Leonardo board
#include "eurotools-v2.h"
#include "mcp4822.h"

// set up pin config
int PIN_CLKIN = A2;
int PIN_POT1 = A0;
int PIN_POT2 = A1;
int PIN_CS = 11;
int PIN_SCK = 10;
int PIN_SDI = 9;
int PIN_LDAC = 0; // grounded in this module

// set up hardware constants
int R1_VALUE = 220;
int R2_VALUE = 150;
bool REVERSE_POT = true;

// set up to read input
bool current_clk = false;
int pot1_pct = 0;
int pot2_pct = 0;
int init_volt = 0;
int decay_rate = 0;

// set up program logic
bool decay = false;
bool clk_reset = false;
int current_offset = 0;
int millis_passed = 0;
int millis_last_update = 0;
bool dac_codeA[16];
bool dac_codeB[16];
int cv_outA_old = 0;
int cv_outA = 0;
int cv_outB_old = 0;
int cv_outB = 0;

// toggle debug
bool debug = false;

void setup() {
  
  // set up all PINS for the chip
  if(debug) Serial.begin(9600);
  pinMode(PIN_CLKIN, INPUT);
  pinMode(PIN_POT1, INPUT);
  pinMode(PIN_POT2, INPUT);
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_SCK, OUTPUT);
  pinMode(PIN_SDI, OUTPUT);
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

  // map to 500-4000
  init_volt = (35 * pot1_pct) + 500;
  if(debug){
    Serial.print ("Pot 1 value: ");
    Serial.println (init_volt);
  }

  // map to 1-100
  decay_rate = max(pot2_pct, 1);
  if(debug){
    Serial.print ("Pot 2 value: ");
    Serial.println (decay_rate);
  }

  //////////////////////////////////////////////////////////////////
  ///// RUN PROGRAM LOGIC
  //////////////////////////////////////////////////////////////////

  // only run once per clock cycle
  // run on rising edge of incoming clock signal
  // while clock is received, do NOT decay!
  if(current_clk & clk_reset){
    
    decay = false;
    clk_reset = false;
  }

  // only run once per clock cycle
  // run on falling edge of incoming clock signal
  // once clock falls, start decay!
  if(!current_clk & !clk_reset){

    decay = true;
    current_offset = 0;
    clk_reset = true;
  }

  // decay voltage while decay flag is TRUE
  cv_outA_old = cv_outA;
  cv_outB_old = cv_outB;
  millis_passed = millis() - millis_last_update;
  if(decay){
    
    // decay every X millis
    if(millis_passed > decay_rate){

      // record cumulative decay
      current_offset = current_offset + (.005 * init_volt);
      cv_outA = init_volt - current_offset;
      cv_outB = current_offset;
      millis_last_update = millis();
      
      // stop decay if limit is reached
      if(cv_outA <= 0 | cv_outB >= init_volt){
        cv_outA = 0;
        cv_outB = init_volt;
        decay = false;
      }
    }
    
  }else{
    
    if(current_clk){
      cv_outA = init_volt; // if no decay, A is HIGH when clock is TRUE
      cv_outB = 0; // if no decay, B is LOW when clock is TRUE
    }else{
      cv_outA = 0;
      cv_outB = init_volt;
    }
  }

  //////////////////////////////////////////////////////////////////
  ///// WRITE OUTPUT
  //////////////////////////////////////////////////////////////////

  if(cv_outA != cv_outA_old){
    update_dac_code(dac_codeA, cv_outA, false, debug);
    write_dac_code(dac_codeA, PIN_CS, PIN_SCK, PIN_SDI, PIN_LDAC);
  }

  if(cv_outB != cv_outB_old){
    update_dac_code(dac_codeB, cv_outB, true, debug);
    write_dac_code(dac_codeB, PIN_CS, PIN_SCK, PIN_SDI, PIN_LDAC);
  }
  
  if(debug) delay(250);
}
