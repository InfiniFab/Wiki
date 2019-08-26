The technical solution explained
################################

The Arduino code
----------------

You can download in the following the arduino code used, every function is commented

:download:`Arduino code <../../_static/pralnia/deedu.ino>`.


The setup
---------

**Moisture Sensor calibration**
The value of the moisture sensor is important because it is its value that will command pump watering

*MoistureLow* variable to be modified to change water pump behavior

**The water pump timing**
If the water tank became completely empty, the water pump is difficult to start again, therefore the solution to prevent it is to control more or less precisely the volume of water sprayed to the plant.
The volume sprayed depend dirrectly on the timing on which the water pump in ON during water spray cycle.
Therefore it is need to know what is the rule between the time the pump is ON and the water volume sprayed, and also the volume of water the tank can contains.

*timePumpPeriod* is the variable in milliseconds of the arduino code to be modified regarding the volume of water sprayed. regarding code in arduino, the pump will be ON the third of the timing defined

**Timing of display**
There is a small timing netween each display of the screen, because clear function of the LCD is called each time. Also a timing is introduced to display a certain amount of time the 2nd screen.

*timePeriod* timing in millisecond that define timing between each refresh of the screen

*timeDisplayPeriod* timing in milliseconds that define the timing the second screens are displayed


The connectic
-------------

:download:`Frtizing schematics <../../_static/pralnia/deedo.fzz>`

**The connection of Arduino on Water pump Alimentation**
The Arduino is to be connected directly on the water pump alimentation so the whole system need only one electric plug to works.

The Water pump is a 12V alimentation, the arduino can support that voltage but this lead to a warming of internal componant of the arduino and possibly damage on the arduino with time.

**solution with a simple dividing bridge**
A simple solution with resistor to get 12V from water pump alimentation an approximatly 9V alimentation for the Arduino is proposed.
In the schematic, the resistor **ResAlim** will determine the alimention of the Arduino.
Empiric value with a potentiometer lead to a maximum value of XXX Ohms to power the Arduino

--> difficult to get precise value on this Resistor

**Best is to go for a tension regulator**

`A page presenting the different solutions <https://blog.yavilevich.com/2017/03/efficient-dc-12v-to-5v-conversion-for-low-power-electronics-evaluation-of-six-modules/>`_
