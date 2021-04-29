import time
from adafruit_servokit import ServoKit  # Set channels to the number of servo channels on your kit.
kit = ServoKit(channels=8) # 8 servo connectors on the robot
kit.frequency = 50

while 1:
    kit.servo[0].angle = 180  # Servo in slot 1 on robot
    time.sleep(1)
    kit.servo[0].angle = 0
    time.sleep(1)