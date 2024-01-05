from gpiozero import LED
from time import sleep

led = LED(17)
counter = 0

while (counter < 5):
    led.on()
    sleep(1) # in seconds
    led.off()
    sleep(1)
    counter += 1