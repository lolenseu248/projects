import cv2
import numpy as np
import mss
import mss.tools


while True:

    # Capture the screen using MSS
    with mss.mss() as sct:
        # Get information of monitor 1
        monitor = sct.monitors[1]
        # Capture the screen
        screenshot = sct.grab(monitor)
        # Convert to a numpy array
        img = np.array(screenshot)

    # Load the picture you want to detect
    template = cv2.imread('fiber5.png', cv2.IMREAD_COLOR)
    template_gray = cv2.cvtColor(template, cv2.COLOR_BGR2GRAY)
    w, h = template_gray.shape[::-1]

    # Convert the captured screen to grayscale
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # Perform template matching
    res = cv2.matchTemplate(img_gray, template_gray, cv2.TM_CCOEFF_NORMED)
    threshold = 0.8
    loc = np.where(res >= threshold)

    # Draw a green box around the detected area
    for pt in zip(*loc[::-1]):
        cv2.rectangle(img, pt, (pt[0] + w, pt[1] + h), (0, 255, 0), 2)

    # Display the result
    cv2.imshow('Detected', img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
