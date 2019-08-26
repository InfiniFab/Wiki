import pycom
import socket
import ubinascii
import binascii
import struct
from machine import Pin, ADC
import time
from onewire import DS18X20, OneWire
from binascii import hexlify, unhexlify
from deepsleep import DeepSleep
import deepsleep
from network import LoRa
import gc

# TIME FOR DEEPSLEEP
deepSleepHours = 2
deepSleepSeconds = deepSleepHours*60*60

# OUTPUT PINS
p1_out = Pin('P8', mode=Pin.OUT)    # g15 expansion board
p2_out = Pin('P9', mode=Pin.OUT)    # g16 expansion board
p3_out = Pin('P22', mode=Pin.OUT)   # g17 expansion board
p4_out = Pin('P11', mode=Pin.OUT)   # g22 expansion board
hum_out = Pin('P12', mode=Pin.OUT)  # g20 expansion board

p1_out.value(0)
p2_out.value(0)
p3_out.value(0)
p4_out.value(0)
hum_out.value(1)

# INPUT PINS
adc = ADC(0)
hum_val = adc.channel(pin='P13')                # g5  expansion board
lev_in = Pin('P20', Pin.IN, Pin.PULL_DOWN)      # g7 expansion board
battery_val = adc.channel(pin='P19', attn=3)    # g6  expansion board
temperature_val = 'P21' #P21'                         # g8  expansion board

# DS18B20 TEMPERATURE SENSOR
ow = OneWire(Pin(temperature_val))
temp = DS18X20(ow)

# GARBAGE COLLECTOR
gc.enable()

# DEEP SLEEP SHIELD
ds = DeepSleep()

# SAVE VALUE
for numeroOrto in range(1, 5):
    if (pycom.nvs_get('count%s' %numeroOrto) == None):
            pycom.nvs_set('count%s'%numeroOrto, 0)
    if (pycom.nvs_get('pausa%s' %numeroOrto) == None):
            pycom.nvs_set('pausa%s'%numeroOrto, 0)
    if (pycom.nvs_get('pompa%s' %numeroOrto) == None):
            pycom.nvs_set('pompa%s'%numeroOrto, 0)

# LORAWAN
lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)
dev_addr = struct.unpack(">l", binascii.unhexlify('26011783'))[0]
nwk_swkey = ubinascii.unhexlify('CFEFE657C90C2BFE2A6D9ECCEA4B1111')
app_swkey = ubinascii.unhexlify('A4D90348A3A516CEB1288C54C54154B0')

for i in range(3, 16):                                                      # remove all the non-default channels
    lora.remove_channel(i)

lora.add_channel(index=0, frequency=868100000, dr_min=0, dr_max=5)          # set the 3 default channels to the same frequency (868.1 MHz)
lora.add_channel(index=1, frequency=868100000, dr_min=0, dr_max=5)
lora.add_channel(index=2, frequency=868100000, dr_min=0, dr_max=5)

lora.join(activation=LoRa.ABP, auth=(dev_addr, nwk_swkey, app_swkey), timeout=0)    # loRa join TTN

s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)          # loRa socket
s.setsockopt(socket.SOL_LORA, socket.SO_DR, 5)
time.sleep(1)

s.setblocking(False)
pycom.heartbeat(False)

# AVERAGE HUMIDITY
time.sleep(2)
humidity = 0
for n1 in range(3):                                 #read values 3 times
    humidity = humidity + (100 - int(hum_val()/40.95))
    time.sleep(1)
humidity = int(humidity/3)
humidity = round(humidity, 2)
time.sleep(2)

# AVERAGE BATTERY VOLTAGE, LEVEL AND TEMPERATURE
level = temperature = batt = 0
for n2 in range(3):                                     #read values 3 times
    batt += battery_val()
    level += lev_in.value()
    #nowTemp = 5
    nowTemp = int(temp.read_temp_async())
    time.sleep(1)
    temp.start_conversion()
    time.sleep(1)
    while nowTemp == 8500:
        nowTemp = int(temp.read_temp_async())
        time.sleep(1)
        temp.start_conversion()
        time.sleep(1)

    time.sleep(1)
    temperature = temperature + nowTemp
    time.sleep(1)

batt = batt/3                       # average battery voltage
#batt = int(round(batt, 2))
adjust_out = 1.06   #258  #1.165
battery_vout = (3.3/4095.0)*batt
battery_vin = (battery_vout/(82/(82+270)))*10*adjust_out
battery_vin = int(round(battery_vin, 2))
print("battery: %s" %battery_vin)

level = level/3                     # average level
level = int(round(level, 2))
print("level: %s" %level)

temperature = temperature/3         # average temperature
temp1 = int(temperature/100)
temp2 = int(temperature%100)
print("temperature: %s.%s" %(temp1, temp2))

pycom.rgbled(0x00007f)
# SEND DATA TROUGH LORA
s.bind(1)                           # lora port
s.setblocking(True)                 # make the socket blocking
#s.send(bytes([temp1, temp2, humidity, level, battery_vin, 0]))
s.send(bytes([temp1, temp2, humidity, level, battery_vin, 0]))
pycom.heartbeat(False)

# READ DATA FROM LORA
s.setblocking(False)
received = s.recv(64)
data = hexlify(received)

#data = b"010906011204010101020202"
#data = b"01141E01241A013626013F11"
if (data != b''):                                               # if i've received new values
    i = j = 1
    for orto in range(1, 5):
        stato = int(data[j:i+1],16)
        if (stato == 2): stato = 1
        pycom.nvs_set('stato%s' %orto, stato)                   # save stato
        pycom.nvs_set('pompa%s' %orto, int(data[i+1:i+3],16))   # save ON time (seconds)
        ore = int(data[i+3:i+5],16)
        giriPausa = (ore)/deepSleepHours     # calc how many times lopy has to deepSleep before irrigation
        pycom.nvs_set('pausa%s' %orto, int(giriPausa))          # save deepSleep round
        i += 6
        j = i-1

    # SEND CONFIRM TROUGH LORA
    s.send(bytes([temp1, temp2, humidity, level, battery_vin, 1]))
    time.sleep(5)
    s.send(bytes([temp1, temp2, humidity, level, battery_vin, 1]))      # another time, for higher security

# CHECK IF I HAVE TO IRRIGATE
for numeroOrto in range(1, 5):                                      # for every garden
    print("--------")
    #print("numeroOrto: %s" %numeroOrto)

    #print("stato%s: %s" %(numeroOrto, pycom.nvs_get('stato%s' %numeroOrto)))
    #print("pompa%s: %s" %(numeroOrto, pycom.nvs_get('pompa%s' %numeroOrto)))
    print("pausa%s: %s" %(numeroOrto, pycom.nvs_get('pausa%s' %numeroOrto)))
    print("count%s: %s" %(numeroOrto, pycom.nvs_get('count%s' %numeroOrto)))

    if (pycom.nvs_get('stato%s' %numeroOrto) != 0):                 # water the garden only if garden is tuned on (setted on node-red)
        if (pycom.nvs_get('count%s' %numeroOrto) == None):          # if counter isn't declare,
            pycom.nvs_set('count%s'%numeroOrto, 0)                  # then counter = 0

        if (pycom.nvs_get('pausa%s' %numeroOrto) <= pycom.nvs_get('count%s' %numeroOrto)):    # if pausa <= counter
            pycom.nvs_set('count%s'%numeroOrto, 0)                                            # then counter = 0

        if (pycom.nvs_get('count%s' %numeroOrto) == 0):             # if counter = 0,
            if (level != 0):                                        # then check if there's water. If there is
                pompa = pycom.nvs_get('pompa%s' %numeroOrto)
                if (pompa != 0):
                    print("Orto_%s: avvio %s secondi di irrigazione" %(numeroOrto, pompa))
                    pinOut = 'p%s_out' %numeroOrto
                    eval(pinOut).value(1)                               # power on the water pump
                    time.sleep(pompa)                                   # wait irrigation time (seconds)
                    print("spengo pompa")
                    eval(pinOut).value(0)                               # turn off the water pump
                    pycom.nvs_set('count%s'%numeroOrto, 1)              # if irrigation is complete, then increase counter
                else: print("Nessun dato ricevuto")
            else: print("Non posso irrigare, SERBATOIO VUOTO")      # if there isn't water, print an error message
        else:
            pycom.nvs_set('count%s'%numeroOrto, (pycom.nvs_get('count%s' %numeroOrto) + 1))     # if isn't time to irrigate, increase counter

    else: print("Orto_%s: SPENTO" %(numeroOrto))                    # if garden is turned off, print a message

    time.sleep(1)                                                   # wait a second before start with a new garden

ds.disable_pullups(['P10','P17', 'P18'])
ds.disable_wake_on_raise(['P10','P17', 'P18'])
ds.disable_wake_on_fall(['P10','P17', 'P18'])
#ds.go_to_sleep(2)
print("Sleep for %s seconds" %(deepSleepSeconds))
gc.collect()
ds.go_to_sleep(int(deepSleepSeconds))       # deepSleep for deepSleepHours*60*60 seconds
#ds.go_to_sleep(3600)
