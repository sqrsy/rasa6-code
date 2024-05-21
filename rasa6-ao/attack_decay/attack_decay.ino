#include "EuroStep.h"

class make_Envelope_AD : public EuroStep::EuroStep {
public:

  int attack_rate;
  int decay_rate;
  bool attack_in_progress = false;
  bool decay_in_progress = false;
  bool end_of_decay = false;
  int envelope_size = 0;
  int end_of_decay_flag = 0;

  ///////////////////////////////////////////////////////////////////////////////
  /// On clock rise, start a new envelope
  ///////////////////////////////////////////////////////////////////////////////

  void on_clock_rise_do() {

    // handle when an envelope is interrupted by a new envelope
    while (envelope_size > 0) {
      envelope_size -= 40;  // decay envelope quickly but not instantly
      if (envelope_size > 0) {
        send_to_output(0, envelope_size);
        write_outputs();
      }
      delayMicroseconds(1);
    }

    envelope_size = 0;          // reset the envelope
    end_of_decay_flag = 0;      // reset end of decay flag
    attack_in_progress = true;  // re-start attack
    decay_in_progress = false;
    end_of_decay = false;
    reset_timer();
  }

  ///////////////////////////////////////////////////////////////////////////////
  /// On each step, check for attack, decay, and end of decay phases
  ///////////////////////////////////////////////////////////////////////////////

  void on_step_do() {

    attack_rate = max(pot_values[0], 1);  // attack rate ranges 1-100
    if (attack_in_progress) {
      if (debug) Serial.println("Attack stage.");
      if (get_timer() > attack_rate) {
        envelope_size += .01 * 4000;
        reset_timer();
      }
      if (envelope_size > 4000) {
        envelope_size = 4000;        // cap envelope size at 4 volts
        attack_in_progress = false;  // stop attack
        decay_in_progress = true;    //start decay
      }
    }

    decay_rate = max(pot_values[1], 1);  // decay rate ranges 1-100
    if (decay_in_progress) {
      if (debug) Serial.println("Decay stage.");
      if (get_timer() > decay_rate) {
        envelope_size -= .01 * 4000;
        reset_timer();
      }
      if (envelope_size < 0) {
        envelope_size = 0;
        decay_in_progress = false;
        end_of_decay = true;
      }
    }

    if (end_of_decay) {
      if (debug) Serial.println("End of decay.");
      end_of_decay_flag = 4000;  // while in EOD, set EOD flag to 4 volts
      if (get_timer() > 50) {    // after 50 ms, end EOD event
        end_of_decay = false;
      }
    } else {
      end_of_decay_flag = 0;  // turn off flag when not in EOD
    }

    // by writing to output_values, the values will get written
    //  to DAC via the write_outputs() method within step()
    send_to_output(0, envelope_size);
    send_to_output(1, end_of_decay_flag);
  }
};

make_Envelope_AD module;

void setup() {
  module.input_mode_is_analog[0] = true;
  module.enable_clock_events(0);
  module.output_mode_is_analog[0] = true;
  module.output_mode_is_analog[1] = true;
  module.debug = false;
  module.start();
}

void loop() {
  module.step();
}
