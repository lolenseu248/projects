import cv2
import numpy as np
import mss

# Screen capture size and location
x, y, width, height = 180, 150, 1600, 780  # Adjust these values according to your screen capture area

# Load reference images and convert them to grayscale
reference_images = ['t4-ore-1.png', 't4-ore-2.png', 't4-ore-3.png', 't3-ore-1.png', 't3-ore-2.png', 't3-ore-3.png']  # Updated reference images
templates = [cv2.imread(img, cv2.IMREAD_GRAYSCALE) for img in reference_images]  # Convert images to grayscale

# Create an instance of mss
with mss.mss() as sct:
    # Define the monitor region to capture
    monitor = {"top": y, "left": x, "width": width, "height": height}

    while True:
        # Capture the screen
        screenshot = sct.grab(monitor)
        frame = np.array(screenshot)

        # Convert frame to grayscale for matching (skip color conversion)
        frame_gray = cv2.cvtColor(frame, cv2.COLOR_BGRA2GRAY)

        # Track object using template matching
        for template in templates:
            result = cv2.matchTemplate(frame_gray, template, cv2.TM_CCOEFF_NORMED)
            min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(result)
            
            # Threshold for detection (adjust according to your needs)
            if max_val > 0.8:
                w, h = template.shape[::-1]
                top_left = max_loc
                bottom_right = (top_left[0] + w, top_left[1] + h)
                cv2.rectangle(frame, top_left, bottom_right, (0, 255, 0), 2)

        # Display the result
        cv2.imshow('Screen Capture', frame)

        # Break the loop on 'q' key press
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

# Clean up
cv2.destroyAllWindows()
