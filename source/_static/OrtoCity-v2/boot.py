import machine
from network import WLAN
import pycom

pycom.heartbeat(False)
pycom.rgbled(0x7f0000)

if (False):
    wlan = WLAN() # get current object, without changing the mode

    if machine.reset_cause() != machine.SOFT_RESET:
        wlan.init(mode=WLAN.STA)
        # configuration below MUST match your home router settings!!
        wlan.ifconfig(config=('192.168.0.111', '255.255.255.0', '192.168.0.1', '8.8.8.8'))

    if not wlan.isconnected():
        # change the line below to match your network ssid, security and password
        wlan.connect('plumake', auth=(WLAN.WPA2, 'wiplumfi13'), timeout=5000)
        while not wlan.isconnected():
            machine.idle() # save power while waiting


