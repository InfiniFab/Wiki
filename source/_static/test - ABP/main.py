#!/usr/bin/env python
#
# Copyright (c) 2019, Pycom Limited.
#
# This software is licensed under the GNU GPL version 3 or any
# later version, with permitted additional terms. For more information
# see the Pycom Licence v1.0 document supplied with this file, or
# available at https://www.pycom.io/opensource/licensing
#

from network import LoRa
import pycom
import socket
import binascii
from binascii import hexlify, unhexlify
import struct
import time
import config
from machine import Pin, ADC

# initialize LoRa in LORAWAN mode.
# Please pick the region that matches where you are using the device:
# Asia = LoRa.AS923
# Australia = LoRa.AU915
# Europe = LoRa.EU868
# United States = LoRa.US915
lora = LoRa(mode=LoRa.LORAWAN, region=LoRa.EU868)

# create an ABP authentication params
dev_addr = struct.unpack(">l", binascii.unhexlify('260119FC'))[0]
nwk_swkey = binascii.unhexlify('580E5E94F1E59A8670BBEDF4008473F8')
app_swkey = binascii.unhexlify('3C532EBC0666AC37D5616D64272D42A3')

#Output Pin
hum_out = Pin('P12', mode=Pin.OUT)  # g20 expansion board
hum_out.value(1)

p1_out = Pin('P8', mode=Pin.OUT)    # g15 expansion board
p1_out.value(0)
#declaring resistor input
adc = ADC(0)
res_input=adc.channel(pin='P13')

#declaring for waterlever detector
lev_in = Pin('P20', Pin.IN, Pin.PULL_UP)      # g7 expansion board





# remove all the non-default channels
for i in range(3, 16):
    lora.remove_channel(i)

# set the 3 default channels to the same frequency
lora.add_channel(0, frequency=config.LORA_FREQUENCY, dr_min=0, dr_max=5)
lora.add_channel(1, frequency=config.LORA_FREQUENCY, dr_min=0, dr_max=5)
lora.add_channel(2, frequency=config.LORA_FREQUENCY, dr_min=0, dr_max=5)

# join a network using ABP (Activation By Personalization)
lora.join(activation=LoRa.ABP, auth=(dev_addr, nwk_swkey, app_swkey))

# create a LoRa socket
s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)

# set the LoRaWAN data rate
s.setsockopt(socket.SOL_LORA, socket.SO_DR, config.LORA_NODE_DR)

# make the socket non-blocking
s.setblocking(False)




for i in range (200):
    #pkt = b'PKT #' + bytes([i])

    #print('Sending:', pkt)
    #val_res=bytes([res_input()/100])
    #val_res=struct.pack('i',res_input())
    val_res=res_input()
    levier=lev_in.value()
    val_send=struct.pack('ii',val_res,levier)
    print("resistance lue:", val_res)
    print("resistance byte:", val_send)


    print("water level:",lev_in.value())
    """
    if (lev_in.value()==0):
        p1_out.value(1)
    else:
        p1_out.value(0)
    """
    # READ DATA FROM LORA
    s.bind(1)
    #s.setblocking(False)
    #received = s.recv(64)
    #data = hexlify(received)
    #print("data:",data)
    s.setblocking(False)
    #s.send(bytes([1, 2, 3]))
    time.sleep(3.0) # wait for the tx and rx to complete
    #rx_pkt = s.recv(64)   # get the packet received (if any)
    """
    try:
        rx_pkt = s.recv(64)   # get the packet received (if any)
        print("pkt:",rx_pkt)
    except socket.timeout:
        print('No packet received')
        """
    #print(type(res_input()))
    #resi[0]=12
    #s.send(val_res)
    s.send(val_send)
    time.sleep(4)
    #s.send(val_send)
    rx, port = s.recvfrom(256)
    if rx:
        print('Received: {}, on port: {}'.format(rx, port))
        print("rx :",rx[0])
        if rx[0]==1:
            p1_out.value(1)
        elif rx[0]==0:
            p1_out.value(0)
    time.sleep(6)
