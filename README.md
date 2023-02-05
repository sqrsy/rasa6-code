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

* Mini Euclid:
	* Author: sqrsy
	* Purpose: Generates simple Euclidean rhythms.
	* Jack 1: Clock in.
	* Pot 1: The number of beats per rhythm necklace (1-8).
	* Pot 2: The length of the rhythm necklace (1-8).
	* Jack 2: The beats of the Euclidean rhythm.
	* Jack 3: The rests of the Euclidean rhythm.

* Poly Rhythm:
	* Author: sqrsy
	* Purpose: Generates polyrhythms by sub-dividing the clock signal.
	* Jack 1: Clock in.
	* Pot 1: The number of sub-divisions for Chan A (1-8).
	* Pot 2: The number of sub-divisions for Chan B (1-8).
	* Jack 2: Chan A out.
	* Jack 3: Chan B out.

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

* Random Walk:
	* Author: sqrsy
	* Purpose: Takes a CV and adds or subtracts a random increment.
	* Jack 1: CV in.
	* Pot 1: The size of the random increment (20-320 mV).
	* Pot 2: The frequency of the random increment (20-320 ms).
	* Jack 2: CV in plus the random increment.
	* Jack 3: CV in minus the random increment.

# Disclaimer

All files are provided without any guarentee. If you notice a mistake, please submit an Issue and I will fix it.