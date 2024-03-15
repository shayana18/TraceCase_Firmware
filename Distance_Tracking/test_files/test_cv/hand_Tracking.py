import numpy as np
from cvzone.HandTrackingModule import HandDetector
import cv2
from picamera2 import Picamera2 as picam
import math 
from numpy import polynomial
import cvzone


piCam = picam() #creating piCamera object
piCam.preview_configuration.main.size = (640,360) #configuring image size
piCam.preview_configuration.main.format = "RGB888" #setting format required for cv2
piCam.preview_configuration.align()
piCam.configure("preview") #applies preview configuration 
piCam.start()

#hand detector

dectector = HandDetector(detectionCon= 0.8, maxHands= 1)
x = [300,245,200,170,145,130,112,103,93,87,80,75,70,67,62,59,57]
y = [20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100]

#outputs coefficents for polynomial equation of type Ax^2 + Bx + C
transform_coeff = np.polyfit(x,y,2)

try:

    while True: 
        frame = piCam.capture_array()
        hands, frame = dectector.findHands(frame)

        if hands and len(hands) ==1:

            pointPositions = hands[0]['lmList']
            boundingBox = hands[0]['bbox']
            x1, y1, z1 = pointPositions[5]
            x2, y2, z2 = pointPositions[17]
            distance = int(math.sqrt((y2-y1)**2 + (x2-x1)**2 + (z2-z1)**2))

            A,B,C = transform_coeff
            distance_to_cam = A*(distance**2) + B*(distance) + C
            print(distance_to_cam)
            cvzone.putTextRect(frame, f'{int(distance_to_cam)}, cm', (boundingBox[0], boundingBox[1]))


        cv2.imshow('test', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break


finally:
    piCam.stop()
    cv2.destroyAllWindows()