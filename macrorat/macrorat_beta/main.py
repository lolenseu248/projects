import mss
import cv2
import time
import numpy as np
from pynput.mouse import Button, Controller as MouseController
from pynput.keyboard import Key, Controller as KeyboardController


#var  
monitor1={'top':320,'left':900,'width':70,'height':20}
monitor2={'top':440,'left':828,'width':249,'height':265}

lootof_img=cv2.imread('imgdump/lootof.png',cv2.IMREAD_UNCHANGED)

loop_counter=0
click_counter=0

match_threshold=0.8

lower=np.array([10,50,80])
upper=np.array([50,225,255])

mouse=MouseController()
keyboard=KeyboardController()


#time.sleep(5)
#loop
with mss.mss() as sct:
    while True:
        lootof_screen=np.asarray(sct.grab(monitor1))
        lootof_result=cv2.matchTemplate(lootof_screen,lootof_img,cv2.TM_CCOEFF_NORMED)

        # debug
        #cv2.imshow('lootofscreen',lootof_screen)
        #cv2.imshow('lootofresult',lootof_result)

        _,max_val,_,max_loc=cv2.minMaxLoc(lootof_result)
        lootpixelresult=max_val>=match_threshold

        if lootpixelresult:
            keyboard.press(Key.shift)
            
            lootbox_screen=np.asarray(sct.grab(monitor2))
            lootbox_hsv=cv2.cvtColor(lootbox_screen,cv2.COLOR_BGR2HSV)
            mask=cv2.inRange(lootbox_hsv,lower,upper)

            # debug
            #cv2.imshow('screen',lootbox_screen)
            #cv2.imshow('mask',mask)

            mask_inv=cv2.bitwise_not(mask)
            non_zero_points=cv2.findNonZero(mask_inv)

            if non_zero_points is not None:
                for point in non_zero_points:
                    mouse.position=(monitor2['left']+point[0][0]+20,monitor2['top']+point[0][1]+20)
                    time.sleep(.2)
                    mouse.press(Button.left)
                    time.sleep(.2)
                    mouse.release(Button.left)
                    time.sleep(.2)
                    keyboard.release(Key.shift)
                    click_counter += 1
                    break


        # debug
        print(f'loop: {loop_counter}, items clicked: {click_counter}', end='\r')

        loop_counter += 1
        if loop_counter == 9:
            loop_counter = 0

        # debug
        #time.sleep(1)
        if cv2.waitKey(1) & 0xFF == ord("q"):
            cv2.destroyAllWindows()
            break
