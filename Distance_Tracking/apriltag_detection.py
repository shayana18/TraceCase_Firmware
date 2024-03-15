import apriltag
import cv2
import time
from picamera2 import Picamera2, Preview

piCam = Picamera2() #creating piCamera object
piCam.preview_configuration.main.size = (640,360) #configuring image size
piCam.preview_configuration.main.format = "RGB888" #setting format required for cv2
piCam.preview_configuration.align()
piCam.configure("preview") #applies preview configuration 
piCam.start()

# distance from camera to object(face) measured in centimeters
Known_distance = 47


# width of image in the real world in centimeter 
Known_width = 16.1

# Colors 
GREEN = (0, 255, 0) 
RED = (0, 0, 255) 
WHITE = (255, 255, 255) 
BLACK = (0, 0, 0) 

def detect_apriltag():
	# load the input image and convert it to grayscale
	print("[INFO] loading image...")
	image = piCam.capture_array()
	gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

	# define the AprilTags detector options and then detect the AprilTags
	# in the input image
	print("[INFO] detecting AprilTags...")
	options = apriltag.DetectorOptions(families="tag16h5")
	detector = apriltag.Detector(options)
	results = detector.detect(gray)
	return results,image

def apriltag_data(image): 

	apriltag_width = 0 # making face width to zero 

	# detecting face in the image 
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
			cv2.circle(image, (cX, cY), 5, (0, 0, 255), -1)
			
			# draw the tag family on the image
			tagFamily = r.tag_family.decode("utf-8")
			cv2.putText(image, tagFamily, (ptA[0], ptA[1] - 15),
			cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
			print("[INFO] tag family: {}".format(tagFamily))

		print("[INFO] {} total AprilTags detected".format(len(detect_apriltag())))
		cv2.imshow('apriltag_detection', image)

		if cv2.waitKey(1) & 0xFF == ord('q'):
			break

		
finally:
    piCam.stop()
    cv2.destroyAllWindows()

