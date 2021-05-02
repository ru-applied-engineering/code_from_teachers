import serial
import time
serialPort = serial.Serial(port = "/dev/ttyS0", baudrate=38400,
                           bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)




serialPort.write(b'((AF40))') #Both motors forward at 40% duty cycle
time.sleep(2)
serialPort.write(b'((AF50))') #Both motors forward at 50% duty cycle
time.sleep(2)
serialPort.write(b'((AR40))') #Both motors in reverse at 40% duty cycle
time.sleep(2)
serialPort.write(b'((RF60))') #Right motor forward at 60% duty cycle, no change for left motor
time.sleep(2)
serialPort.write(b'((LF050))') # Left motor forward at 50% duty cycle, not change for right motor
time.sleep(2)
serialPort.write(b'((AR000))') # Stop both motors


serialPort.close()