from gpiozero import LED
from time import sleep

led = LED(17)
count = 0 

while (count < 5):
    led.on()
    sleep(1)
    led.off()
    sleep(1)
    count+= 1