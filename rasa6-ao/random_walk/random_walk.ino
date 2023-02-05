// for beetle, setup with Leonardo board
#include "eurotools-v2.h"
#include "mcp4822.h"

// set up pin config
int PIN_CVIN = A2;
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
int cv_in = 0;
int cv_in_history[8];
int pot1_mV = 0;
int pot2_mV = 0;
int random_magnitude = 0;
int random_interval = 0;

// set up program logic
int millis_new = 0;
int millis_ref = 0;
int millis_track = 0;
int cv_offset = 0;
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
  pinMode(PIN_CVIN, INPUT);
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

  // read in CV
  cv_in = read_analog_mV_smooth(PIN_CVIN, cv_in_history, R1_VALUE, R2_VALUE, debug);

  // convert first pot voltage into integer range 0-15 => map to 20-320
  pot1_mV = read_analog_mV(PIN_POT1, 0, 0);
  random_magnitude = 16 - mV_to_integer(pot1_mV, 15, 1, 5); // integer ranges 0-15
  random_magnitude = 20*random_magnitude;
  if(debug){
    Serial.print ("Pot 1 value: ");
    Serial.println (random_magnitude);
  }

  // convert second pot voltage into integer range 0-15 => map to 20-320
  pot2_mV = read_analog_mV(PIN_POT2, 0, 0);
  random_interval = 16 - mV_to_integer(pot2_mV, 15, 1, 5); // integer ranges 0-15
  random_interval = 20*random_interval;
  if(debug){
    Serial.print ("Pot 2 value: ");
    Serial.println (random_interval);
  }

  //////////////////////////////////////////////////////////////////
  ///// RUN PROGRAM LOGIC
  //////////////////////////////////////////////////////////////////

  // track time since last step in random walk
  millis_new = millis();
  millis_track = millis_new - millis_ref;

  // every X seconds, take one step in the random walk
  // there is a single 'offset' value that will increment or decrement
  //  according to the random magnitude pot value
  if(millis_track > random_interval){
    if(random(1, 10) > 5){
      cv_offset = cv_offset + random_magnitude;
    }else{
      cv_offset = cv_offset - random_magnitude;
    }
    cv_offset = min(cv_offset, 1600);
    cv_offset = max(cv_offset, -1600);
    millis_ref = millis_new;
  }

  // update CV out based on the offset
  // ChanA is input + offset
  // ChanB is input - offset
  cv_outA_old = cv_outA;
  cv_outA = cv_in + cv_offset;
  cv_outB_old = cv_outB;
  cv_outB = cv_in - cv_offset;

  if(debug){
    Serial.print ("CV offset: ");
    Serial.println (cv_offset);
    Serial.print ("CV out A old: ");
    Serial.println (cv_outA_old);
    Serial.print ("CV out B old: ");
    Serial.println (cv_outB_old);
    Serial.print ("CV out A: ");
    Serial.println (cv_outA);
    Serial.print ("CV out B: ");
    Serial.println (cv_outB);
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
