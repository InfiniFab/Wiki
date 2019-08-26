Specifications and Conception Choice of the Deedu Plant
#######################################################

Main objectives of the plant
----------------------------

The Deedu plant is a system than allow control and automatic watering of a small plant.
This system is developped for an educationnal purpose, the plant shall stay at different pupils house to be controlled.
Every time value controlled by the system will be entered in a database.

What does it control ?
++++++++++++++++++++++

* The moisture of the soil, this value will be directly used to water or not the plant
* The CO2 gas concentration
* The intensity of the light
* The temperature and the humidity of ambiant air

What action can be performed ?
++++++++++++++++++++++++++++++

* A water pump will get water from a small reservoir to put it on the plant
* A screen will display the informations needed and 2 buttons to control what info to be displayed


Material used
-------------

The stand
+++++++++

The stand will contains all the system:

* the electronic control and command part
* the water tank
* the plant pot

It shall be made simply with wood plank


The Command&Control part
++++++++++++++++++++++++

The material used for the Command and Control are the following:

* An arduino UNO for all the logic
* A CO2 Gas sensor
* A moister sensor for the earth of the plant
* A DHT11 sensor for the temperature and the humidity of the ambiant air
* A photo Resistor to measure light intensity
* A DC relay to comman the water pump
* A small water pump designed for aquarium
* 2 buttons normally open
* A LED to warn user about fill water tank
* 3 1kOhm resistor for the buttons and the LED

The use cases
-------------

In this sections is presented how shall be used Deedu system and what shall be the response for it at each cases

UC01 : Control data from sensors & Number of spraying
+++++++++++++++++++++++++++++++++++++++++++++++++++++

By using one button and looking on the screen, the user shall be able to check on instant value measured by all sensors.

.. figure :: ../../_static/pralnia/UC01.bmp

UC02 : Spray the plant when needed & Water tank level control
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The system shall be able to water plant when it is needed (The moisture sensor value indicated watering is needed).

The water pump is tricky to start (if no water is present in tube, it isn't powerfull enough to start), so the best is to prevent complete emptiness of the water tank.
Therefore a solution shall be present to control water tank level and prevent water pumping if water level is too low. A warning shall indicate the user to fill the water tank.

.. figure :: ../../_static/pralnia/UC02.bmp
