# What is it?

The 'Rasa6' is a 6HP Eurorack module built around the Arduino Beetle from DFRobot. The circuit is very simple, but can be used to perform a number of tasks helpful in music synthesis. My intent was to design a standard module that could be easily programmed by anyone in the DIY music scene, even those with limited software or hardware skills. All code is open source, freely available for you to use and modify. If you make a cool module using the 'Rasa6' module -- or some cool music -- let me know and I will link it here.

# About the 'Rasa6'

The 'Rasa6' circuit is very simple. It has 3 jacks and 2 pots.
* Jack 1: For reading input. Voltage divider steps 0-12V down to 0-5V so your Arduino doesn't get fried.
* Pots 1 & 2: For setting parameters. Reads 0-5V that can be mapped to module parameters.
* Jacks 2 & 3: For sending output. The analog out (AO) circuit uses an MCP4822 Digital-to-Analog converter (DAC). The digital out (DO) circuit bypasses the MCP4822 with a jumper.

# DO Programs

The following programs are currently available:

* Bernoulli Gate:
	* Author: sqrsy
	* Purpose: Takes a clock signal and sends it randomly.
	* Jack 1: Clock in.
	* Pot 1: The probability of sending the clock to Chan A.
	* Pot 2: The probability of sending the clock to Chan B if not send to Chan A.
	* Jack 2: Chan A out.
	* Jack 3: Chan B out.

* Clock Divider:
	* Author: sqrsy
	* Purpose: Generates new clock signals by dividing the incoming clock signal.
	* Jack 1: Clock in.
	* Pot 1: The number of divisions for Chan A, including non-integer divisions (f/1-f/8).
	* Pot 2: The number of divisions for Chan B, including non-integer divisions (f/1-f/8).
	* Jack 2: Chan A out.
	* Jack 3: Chan B out.

* Clock Multiplier:
	* Author: sqrsy
	* Purpose: Generates new clock signals by multiplying the incoming clock signal.
	* Jack 1: Clock in.
	* Pot 1: The number of multiples for Chan A (1-8).
	* Pot 2: The number of multiples for Chan B (1-8).
	* Jack 2: Chan A out.
	* Jack 3: Chan B out.

* Euclidean Rhythms:
	* Author: sqrsy
	* Purpose: Generates simple Euclidean rhythms.
	* Jack 1: Clock in.
	* Pot 1: The number of beats per rhythm necklace (1-8).
	* Pot 2: The length of the rhythm necklace (1-8).
	* Jack 2: The beats of the Euclidean rhythm.
	* Jack 3: The rests of the Euclidean rhythm.

* Rapid Trigger:
	* Author: sqrsy
	* Purpose: Generates a rapid sequence of gate signals.
	* Jack 1: Clock in.
	* Pot 1: The number of gates in a sequence (1-8).
	* Pot 2: The length of each gate in the sequence (10-80 ms).
	* Jack 2: The rapid sequence of gates (1-8).
	* Jack 3: TRUE when the rapid sequence is underway.

# AO Programs

The following programs are currently available:

* Attack Decay:
	* Author: sqrsy
	* Purpose: Outputs a linear attack-decay (AD) enevelope.
	* Jack 1: Trigger in.
	* Pot 1: The time it takes to attack (~100 ms to ~10 sec).
	* Pot 2: The time it takes to decay (~100 ms to ~10 sec).
	* Jack 2: The AD envelope. Max voltage is 4 volts.
	* Jack 3: A short 4V gate once decay finishes.

* Attack Release:
	* Author: sqrsy
	* Purpose: Outputs a linear attack-release (AR) enevelope.
	* Jack 1: Gate in. Release begins once gate ends.
	* Pot 1: The time it takes to attack (~100 ms to ~10 sec).
	* Pot 2: The time it takes to release (~100 ms to ~10 sec).
	* Jack 2: The AR envelope. Max voltage depends on gate.
	* Jack 3: A short 4V gate once release finishes.

* No Attack Decay:
	* Author: sqrsy
	* Purpose: Takes a trigger signal and decays it immediately.
	* Jack 1: Trigger in.
	* Pot 1: The maximum output voltage (500-4000 mV).
	* Pot 2: The time it takes to decay 0.5% (1-100 ms).
	* Jack 2: The trigger decay.
	* Jack 3: The inverse.

* No Attack Release:
	* Author: sqrsy
	* Purpose: Takes a gate signal and decays it after it finishes.
	* Jack 1: Gate in.
	* Pot 1: The maximum output voltage (500-4000 mV).
	* Pot 2: The time it takes to decay 0.5% (1-100 ms).
	* Jack 2: The gate and its decay.
	* Jack 3: The inverse.

* Random Walk:
	* Author: sqrsy
	* Purpose: Takes a CV and adds or subtracts a random increment.
	* Jack 1: CV in.
	* Pot 1: The size of the random increment (20-320 mV).
	* Pot 2: The frequency of the random increment (20-320 ms).
	* Jack 2: CV in plus the random increment.
	* Jack 3: CV in minus the random increment.

# Disclaimer

All files are provided without any guarantee. If you notice a mistake, please submit an Issue and I will fix it.