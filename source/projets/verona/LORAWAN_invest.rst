Solution via LORAWAN
====================

The hydroponic system is inspired by Ortocity, a system that allows control and watering of the soil of a module culture.
The main inspiration come from usage of LORA technology.

In the following are annotation and investigation of the technology used. In the end, the use of LORAWAN hasn't been choose because WIFI.

Creation of a LORAWAN Gateway
*****************************

Because the previous LORAWAN gateway used by Ortocity wasn't available at the begining of the investigation, a LORAWAN Gateway was created to allow Ortocity to send data.


**The material used**

* A Lopy 4 board

https://docs.pycom.io/datasheets/development/lopy4/

* The expansion board 2.0

https://docs.pycom.io/datasheets/boards/expansion2/

* a WIFI antena

**Process**

:download:`The code to be uploaded on the Lypo  <../../_static/Lorawan nano-gateway.zip>`

On recent computer, driver shall be present and pymark shall detect lypo devices and state connected at beginning, otherwise instal driver, etc … of lopy material :

https://docs.pycom.io/gettingstarted/installation/

Follow the instruction which are pretty basics.

https://docs.pycom.io/tutorials/lora/lorawan-nano-gateway.html

The gateway needs to be registered on the thingnetwork(TTN). in that purpose :

* we create a python script with the code suggested in “Gateway ID” chapter to be run once uploaded
* retrieve the files main.py, config.py and nanogateway.py files from github, and just put local wifi ssid&password into config.py
* just follow throught the instruction on registering your gateway (with gateway ID generated, and the correct frequency&portal from where you are)
* Verify the good reference of the gateway on TTN platform


.. note::

  The main issues come from uploading code into lypo.

  You needs to upload last firmware on the device following the method:
  https://docs.pycom.io/gettingstarted/installation/firmwaretool.html

  during firmware update erase flash on lypo

Interface with Lypo can be done with atom IDE with pymark module installed

.. note::

  make sure the lopy works, with the following simple program :
  https://docs.pycom.io/gettingstarted/programming/first-project.html

  update the firmware
  https://docs.pycom.io/gettingstarted/installation/firmwaretool.html

  with atom and Pymark ad-on, upload lora-gate way code
  https://docs.pycom.io/tutorials/lora/lorawan-nano-gateway.html

  useful video :
  https://www.youtube.com/watch?v=d7CHfmqkVWc


Create Lora device, node
************************

.. warning::

  This can be used to test the LORAWAN gateway when Ortocity isn't available.
  
  **NOT USED IN HYDRO PROJECT**

**Material:**

* 1 Lopy 1 from pycom

https://docs.pycom.io/datasheets/development/lopy.html

* extension board v2

https://docs.pycom.io/datasheets/boards/expansion2.html

* wifi antenna

**Process:**

follow the install instruction for ABP Node
https://docs.pycom.io/tutorials/lora/lorawan-nano-gateway.html
use the file node_ABP.py and the same config.py from github
use also device EUI python script and run to have the Device EUI for device registration on TTN application
create an application on TTN, for the application create a new device, use device ID you want and device EUI from python script
save in node_ABP.py the info from the device created in TTN application.
in device overview
copy device adress into dev_addr
copy Network Session Key into nwk_swkey
copy App Session Key into app_swkey
upload node_ABP.py(renamed into main.py) & config.py into lypo device (thought atom)
in TTN application, in “data” tab verify that data arrives
possible to need to reset Lypo with reset button close to Lypo  LED & reset the frame counter from devices overview page

Informations that are transfered are only a counter for the moment, with the line :
“s.send(pkt)”. We want to test the transfert the value of a potentiometer connected to a PIN of the Lypo. All changes from emetor persepectives happened in main.py

Resistor needs to define an input of the Lypo as variable input :

.. code-block ::

  import ADC librairy “from machine import Pin, ADC”
  declare an ADC object and the input pin of the Lypo
  adc = ADC(0)
  res_input=adc.channel(pin='P13')

!!respect the pinout of extension board
https://docs.pycom.io/.gitbook/assets/expansion2-pinout.pdf
not all pinout can allow IN/OUT connection, for this exemple, didn’t found any clear documentation on the subject, got inspired by code from OrtoCity project
call the measurement of the resistor “res_input()”
Now it is needed to convert the value from an INT to a BYTE that is the only format to be transfered as data through LORA.
import struct librairy for convert whatever variable into byte
https://docs.python.org/3/library/struct.html
a video to understand better struct
https://www.google.com/search?q=struct+pack+tutorial&oq=struct.pack+tut&aqs=chrome.1.69i57j0l2.8730j0j7&sourceid=chrome&ie=UTF-8#kpvalbx=1
convert the measured resistence into a byte

.. code-block :: python

  val_send=struct.pack('i',val_res)

Send the byte
in TTN application, you can add a payload format to display the numerical value of the resistance (instead of the hexa value)

.. code-block :: python

  function Decoder(bytes, port) {
    // Decode an uplink message from a buffer
    // (array) of bytes to an object of fields.
    var decoded = {};
    var resistance = (bytes[1]<<8)|bytes[0];

    // if (port === 1) decoded.led = bytes[0];

    return {
      resistance : resistance,
    }
  }

!!the first byte sent are the first byte recieved, needs to take care of the order in the payload format

Now we’d like to have a NodRed interface to get value from TTN and display it

Install Nodejs
install node-red
install node-red dashboard
run node red
install the “node-red-contrib-ttn” package
!!careful if the internet paserel on which your are connected allows that
you can import ttn exemple
configure ttn uplink node with ttn app name & device ID
in the function node retrieve the resistance value pushed by ttn payload format
“resistance : msg.payload_fields.resistance”
display value of resistance with text node, with value
“{{msg.resistance}}”

Handle IN/OUT on lopy

declare IN/OUT on the right pin

.. code-block :: python

  p1_out = Pin('P8', mode=Pin.OUT)    # g15 expansion board”

!!at beginning were aiming for pin G20, but seems not working
controlling it is pretty easy with python

.. code-block :: python

  if (lev_in.value()==0):
          p1_out.value(1)
      else:
          p1_out.value(0)

here lev_in represent the water level sensor.

Next is to transfer order from Node Red interface to Lopy, a button to turn ON LED
Info are transfered from TTN do device via downlink, now info are send as fields and not payloads

on device, nothing is recieved.
when sending downlink with payload from TTN interface, we can see the payload is recieved

!! needs to understand the limit size and what informations can be send
previously function from node-red was sending fields, which never managed to retrieved on device, and supposed to take bandwith


change the function to send raw payload

to pass switch value to a payload, just put it in serie with passing throught output parameter in switch node
in function, pass switch value simply to raw payload

retrieve payload switch info in python from lypo

pass waterlevelswitch throught TTN to lypo, use function in payload formats from TTN

retrieve water level in node red function :

investigate DB with nodered
