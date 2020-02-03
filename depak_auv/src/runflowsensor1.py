import RPi.GPIO as GPIO
from time import sleep
import time, math,sys,os

dist_meas= 0.0
km_per_hour=0
rpm=0
pulse=0
sensor=2
elapse=0
rate=0
start_time= time.time()

def init_GPIO():
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)
    GPIO.setup(sensor,GPIO.IN,pull_up_down=GPIO.PUD_DOWN)
    
def calculate_time(channel):
    global pulse, start_time, elapse
    pulse=pulse+1
    elapse= time.time()-start_time
    start_time=time.time()
def calculate_speed(r_cm):
    global pulse, elapse, rpm, dist_km, dist_meas, km_per_second, km_per_hour
    if elapse!=0:
        rpm=1/elapse*60
        circ_cm=(2*math.pi)*r_cm
        dist_km=circ_cm*100000
        km_per_second=  dist_km/elapse/1000
        km_per_hour=km_per_second/3600
        dist_meas=(dist_km*pulse)*1000
        return km_per_hour
    
def init_interrupt():
    GPIO.add_event_detect(sensor,GPIO.FALLING,callback = calculate_time,bouncetime =20)

        
if __name__=='__main__':
    init_GPIO()
    init_interrupt()
    while True:
        if GPIO.input(sensor)== 0:
             rpm=0
             km_per_hour=0
             dist_meas=0
             pulse=0
             elapse=0
        
        try:
        
            calculate_speed(3)
            print('rpm:{0:0f}-RPM kmh:{1:.0f}-KMH time:{2}'.format(rpm,km_per_hour,elapse*3600))
            
            sleep(1)
            
        except KeyboardInterrupt:
            GPIO.cleanup()
            sys.exit
    
        
        
                    
