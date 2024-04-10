import threading
import queue
import cv2
import apriltag
from picamera2 import Picamera2
import numpy as np
import time
import math
import serial

#equation calculation 
point_dist = [403.33,369.33,327,297.67,272.33,250.67,234,219.67,205.33,190.67,182,171,165,156,150,143,137,132,125,121,116,112,110,106,102,100,96,94,91,88,86,83,82,77,74,71,69,64]
dist_from_cam = [40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180,185,190,195,200,210, 220,230,240,250]
transform_coeff = np.polyfit(point_dist,dist_from_cam,4)
A,B,C,D,E = transform_coeff

# Initialize queues for image processing and result communication
image_queue = queue.Queue(maxsize=1)  # Set maxsize to control memory usage
result_queue = queue.Queue(maxsize=1)


def setup_camera():
    piCam = Picamera2()
    piCam.preview_configuration.main.size = (640, 360)  # Adjust based on your needs
    piCam.preview_configuration.main.format = "RGB888"
    piCam.preview_configuration.align()
    piCam.configure("preview")
    return piCam

def apriltag_detection_worker():
    print("AprilTag Detection Worker Started")
    detector = apriltag.Detector()
    while True:
        image = image_queue.get()
        if image is None:  # Shutdown signal
            break
        # Convert to grayscale for detection
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        results = detector.detect(gray)
        # Put the detection results back in the queue
        result_queue.put((results, image))

def display_results():
    while True:
        item = result_queue.get()
        ser = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
        ser.reset_input_buffer()
        if item is None:  # Shutdown signal
            result_queue.task_done()
            break
        results, image = item
        for r in results:
            # Draw detected AprilTag outlines
            (ptA, ptB, ptC, ptD) = r.corners
            ptB = (int(ptB[0]), int(ptB[1]))
            ptC = (int(ptC[0]), int(ptC[1]))
            ptD = (int(ptD[0]), int(ptD[1]))
            ptA = (int(ptA[0]), int(ptA[1]))
            (cX, cY) = (int(r.center[0]), int(r.center[1]))
            
            distance_between_points = int(math.sqrt((ptB[0] - ptD[0])**2 + (ptB[1] - ptD[1])**2))
            distance_from_cam = int(A*(distance_between_points**4) + B*(distance_between_points**3) + C*(distance_between_points**2) + D*(distance_between_points) + E)
            string_to_send = str(distance_from_cam) + "," +str(cX) + "\n"
            ser.write(string_to_send.encode('utf-8'))
            print(distance_from_cam)
            
        # Display the frame with detected AprilTags
        cv2.imshow('AprilTag Detection', cv2.cvtColor(image, cv2.COLOR_RGB2BGR))
        if cv2.waitKey(1) & 0xFF == ord('q'):
            cv2.destroyAllWindows()
            break
        result_queue.task_done()

def main():
    threading.Thread(target=apriltag_detection_worker, daemon=True).start()
    threading.Thread(target=display_results, daemon=True).start()

    with setup_camera() as piCam:
        piCam.start()
        try:
            while True:
                frame = piCam.capture_array()
                if not image_queue.full():
                    # Replace the oldest frame
                    try:
                        image_queue.get_nowait()  # Discard the old frame
                    except queue.Empty:
                        pass
                    image_queue.put(frame)
        finally:
            # Send shutdown signal to threads
            image_queue.put(None)
            result_queue.put(None)

if __name__ == "__main__":
    main()
