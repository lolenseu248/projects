import mss
import cv2
import time
import numpy as np
from pynput.mouse import Button, Controller as MouseController

#var
monitor1 = {'top': 320, 'left': 900, 'width': 70, 'height': 20}
monitor2 = {'top': 440, 'left': 828, 'width': 249, 'height': 265}
monitor3 = {'top': 693, 'left': 1543, 'width': 333, 'height': 250}

lootof_img = cv2.imread('imgdump/lootof.png', cv2.IMREAD_UNCHANGED)

lootpixelresult = False

checker = 0
loop_counter = 0
click_counter = 0
scan_x, scan_y = 1855, 920


lowinvin = (1885, 935)
sortinvi = (1830,965)

lower = np.array([10, 50, 80])
upper = np.array([50, 225, 255])

match_threshold = 0.8

mouse = MouseController()


#fuc


#loop
with mss.mss() as sct:
    while True:        
        lootof_screen = np.asarray(sct.grab(monitor1))
        lootof_result = cv2.matchTemplate(lootof_screen, lootof_img, cv2.TM_CCOEFF_NORMED)

        _, max_val, _, _ = cv2.minMaxLoc(lootof_result)
        lootpixelresult = max_val >= match_threshold

        if lootpixelresult:
            mouse.position = lowinvin
            mouse.click(Button.left)
            invi_at_low = True

            lootbox_screen = np.asarray(sct.grab(monitor2))
            lootbox_hsv = cv2.cvtColor(lootbox_screen, cv2.COLOR_BGR2HSV)
            lootbox_mask = cv2.inRange(lootbox_hsv, lower, upper)

            mask_inv = cv2.bitwise_not(lootbox_mask)
            non_zero_points = cv2.findNonZero(mask_inv)

            if non_zero_points is not None:
                for point in non_zero_points:
                    mouse.position = (point[0][0] + monitor2['left'] + 20, point[0][1] + monitor2['top'] + 20)
                    mouse.press(Button.left)
                    time.sleep(1)
                    break

            while scan_y >= 815 and scan_x >= 1590:
                invibox_screen = np.asarray(sct.grab(monitor3))
                invibox_hsv = cv2.cvtColor(invibox_screen, cv2.COLOR_BGR2HSV)
                invi_mask = cv2.inRange(invibox_hsv, lower, upper)

                if invi_mask[scan_y - monitor3['top'], scan_x - monitor3['left']] != 0:
                    mouse.position = (scan_x - 20, scan_y - 20)
                    mouse.release(Button.left)
                    click_counter+=1
                    break

                    


            #mouse.position = sortinvi
            #mouse.click(Button.left)
                    
        else:
            pass

        print(f'loop: {loop_counter}, items clicked: {click_counter}', end='\r')

        loop_counter += 1
        if loop_counter == 9:
            loop_counter = 0

        if cv2.waitKey(1) & 0xFF == ord("q"):
            cv2.destroyAllWindows()
            break
