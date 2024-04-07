import cv2
from picamera2 import Picamera2
from gpiozero import LED


piCam = Picamera2() #creating piCamera object
piCam.preview_configuration.main.size = (640,360) #configuring image size
piCam.preview_configuration.main.format = "RGB888" #setting format required for cv2
piCam.preview_configuration.align()
piCam.configure("preview") #applies preview configuration 
piCam.start() 

face_cascade = cv2.CascadeClassifier("/home/tracecase/TraceCase_Firmware/Distance_Tracking/test_files/test_cv/haarcascade_frontalface_default.xml")

try:

    while True: 
        frame = piCam.capture_array()
        grey_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY) #turning captured frame into gray scale
        faces = face_cascade.detectMultiScale(grey_frame, 1.1, 10) # passing in grey frame to apply pre-trained model on 

        for (x,y,w,h) in faces:
            cv2.rectangle(frame, (x,y), ((x+w), (y+h)), (255,0,0), 2) #adding rectangle around face after pre-trained model recognizes face

        cv2.imshow('Face_Dectection', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break


finally:
    piCam.stop()
    cv2.destroyAllWindows()