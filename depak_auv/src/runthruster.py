import RPi.GPIO as GPIO
from time import sleep

thrusterpin = 5
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(thrusterpin, GPIO.OUT)
pi_pwm = GPIO.PWM(thrusterpin,50)

zero = 7.5

pi_pwm.start(zero)

halfrange = (9.5-zero)/4.0
step = 0.1

currpow = zero
dir = 1

sleep(10)

try:
	while True:
		if (currpow > zero+halfrange) or (currpow < zero-halfrange):
			dir = -dir
		currpow = currpow + dir*step*halfrange
		pi_pwm.ChangeDutyCycle(currpow)
		sleep(0.2)

except KeyboardInterrupt:
	pi_pwm.stop()
	GPIO.cleanup()
