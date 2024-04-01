import apriltag
import cv2
import time
from picamera2 import Picamera2, Preview
import numpy as np 
import math 
import serial

piCam = Picamera2() #creating piCamera object
piCam.preview_configuration.main.size = (640,360) #configuring image size
piCam.preview_configuration.main.format = "RGB888" #setting format required for cv2
piCam.preview_configuration.align()
piCam.configure("preview") #applies preview configuration 
piCam.start()

# Colors 
GREEN = (0, 255, 0) 
RED = (0, 0, 255) 
WHITE = (255, 255, 255) 
BLACK = (0, 0, 0) 

point_dist = [480,392,343,311,284,260,230,220,204,193,183,171,162,155,148,148,141,136,131,127,122,118,112,110,101,94,89,87,80,77,74,71,68,65,63]
dist_from_cam = [35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,160,170,180,190,200,210,220,230,240,250,260]
transform_coeff = np.polyfit(point_dist,dist_from_cam,4)
A,B,C,D,E = transform_coeff

def detect_apriltag():
	# load the input image and convert it to grayscale
	print("[INFO] loading image...")
	image = piCam.capture_array()
	gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

	# define the AprilTags detector options and then detect the AprilTags
	# in the input image
	print("[INFO] detecting AprilTags...")
	#options = apriltag.DetectorOptions(families="tag16h5")
	detector = apriltag.Detector()
	detector.decode_sharpening = 1.0
	detector.refine_edges = 1
	detector.families = "tag16h5"
	#detector = apriltag.Detector(options)
	results = detector.detect(gray)
	return results,image

def apriltag_data(image): 

	# detecting apriltag in the image 
	apriltags = detect_apriltag() 

	# looping through the faces detect in the image 
	# getting coordinates x, y , width and height 
	for (x, y, h, w) in apriltags: 

		# draw the rectangle on the apriltag 
		cv2.rectangle(image, (x, y), (x+w, y+h), GREEN, 2) 

		# getting face width in the pixels 
		face_width = w 

	# return the face width in pixel 
	return face_width 
	
try:
	ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
	ser.reset_input_buffer()
	
	while True:
		results, image = detect_apriltag()

		# loop over the AprilTag detection results
		for r in results:
			# extract the bounding box (x, y)-coordinates for the AprilTag
			# and convert each of the (x, y)-coordinate pairs to integers
			(ptA, ptB, ptC, ptD) = r.corners
			ptB = (int(ptB[0]), int(ptB[1]))
			ptC = (int(ptC[0]), int(ptC[1]))
			ptD = (int(ptD[0]), int(ptD[1]))
			ptA = (int(ptA[0]), int(ptA[1]))
			
			# draw the bounding box of the AprilTag detection
			cv2.line(image, ptA, ptB, (0, 255, 0), 2)
			cv2.line(image, ptB, ptC, (0, 255, 0), 2)
			cv2.line(image, ptC, ptD, (0, 255, 0), 2)
			cv2.line(image, ptD, ptA, (0, 255, 0), 2)
			
			# draw the center (x, y)-coordinates of the AprilTag
			(cX, cY) = (int(r.center[0]), int(r.center[1]))
			cv2.circle(image, (cX, cY), 5, (0, 0, 255), -1) #middle 
			cv2.circle(image, (ptB[0], ptB[1]), 5, (0, 0, 255), -1) #Bottom Right 
			cv2.circle(image, (ptD[0], ptD[1]), 5, (0, 0, 255), -1) # Top left

			#getting distance between poin B and D 
			distance = int(math.sqrt((ptB[0] - ptD[0])**2 + (ptB[1] - ptD[1])**2))
			distance_from_cam = int(A*(distance**4) + B*(distance**3) + C*(distance**2) + D*(distance) + E)
			string_to_send = str(distance_from_cam) + "," +str(cX) + "\n"

			strDist = "Dist: "+ str(distance_from_cam)
			cv2.putText(image, strDist, (cX, cY - 100), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
			
			# draw the tag family on the image
			tagFamily = r.tag_family.decode("utf-8")
			cv2.putText(image, tagFamily, (ptA[0], ptA[1] - 15),
			cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
			print("[INFO] tag family: {}".format(tagFamily))
					
			ser.write(string_to_send.encode('utf-8'))
			line = ser.readline().decode('utf-8').rstrip()
			print(line)
			time.sleep(1)

		print("[INFO] {} total AprilTags detected".format(len(detect_apriltag())))
		cv2.imshow('apriltag_detection', image)


		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

		
finally:
    piCam.stop()
    cv2.destroyAllWindows()

