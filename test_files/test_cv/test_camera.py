import cv2
from picamera2.encoders import H264Encoder, Quality
from picamera2 import Picamera2
import time
import numpy as np

picam2 = Picamera2()
picam2.configure(picam2.create_preview_configuration(main={"format": 'XRGB8888', "size": (640, 480)}))
picam2.start()


while True:
    im = picam2.capture_array()
    # test= cv2.imread("test.jpg")

    grey = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)

    contours = cv2.Canny(im, 100, 200)

    cv2.imshow('Camera', grey)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

# picam2.configure(picam2.create_video_configuration())
# encoder = H264Encoder()
# picam2.start_recording(encoder, '/home/tracecase/test_outputs/test.h264', quality=Quality.LOW)
# time.sleep(10)
# picam2.stop_recording()

