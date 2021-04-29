import serial
import numpy as np
import time
serialPort = serial.Serial(port = "/dev/ttyS0", baudrate=4800,
                           bytesize=8, timeout=10, stopbits=serial.STOPBITS_ONE)


# b'\x01' => Motor 1 stops
# b'\x02' => Motor 1 drives forward
# b'\x03' => Motor 1 drives backwards
# b'\x04' => Motor 2 stops
# b'\x05' => Motor 2 drives forward
# b'\x06' => Motor 2 drives backwards

n = serialPort.write(b'\x02') # \x01 - \x06

serialPort.close()      
