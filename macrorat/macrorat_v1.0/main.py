import time
import cv2 as cv
import numpy as np
from mss import mss
from pynput.mouse import Button, Controller as MouseController
from pynput.keyboard import Key, Controller as KeyboardController


#var 
loop_counter=0
click_counter=0

monitor1={'top':320,'left':900,'width':70,'height':20}
monitor2={'top':440,'left':828,'width':249,'height':265}

sct = mss()

lootof_img=cv.imread('img/trigger/lootof.png',cv.IMREAD_UNCHANGED)

lower=np.array([10,50,80])
upper=np.array([50,225,255])

match_threshold=0.8

mouse=MouseController()
keyboard=KeyboardController()


#func
def clicker(x,y):
    mouse.position=(x,y)
    time.sleep(.02)
    mouse.press(Button.left)
    time.sleep(.08)
    mouse.release(Button.left)
    time.sleep(.08)

def lootof():
    lootof_screen=np.asarray(sct.grab(monitor1))
    lootof_result=cv.matchTemplate(lootof_screen,lootof_img,cv.TM_CCOEFF_NORMED)

    # debug
    #cv.imshow('lootofscreen',lootof_screen)
    #cv.imshow('lootofresult',lootof_result)

    _,max_val,_,max_loc=cv.minMaxLoc(lootof_result)
    return max_val>=match_threshold

def lootbox():
    global click_counter
    lootbox_screen=np.asarray(sct.grab(monitor2))
    lootbox_hsv=cv.cvtColor(lootbox_screen,cv.COLOR_BGR2HSV)
    mask=cv.inRange(lootbox_hsv,lower,upper)

    # debug
    #cv.imshow('screen',lootbox_screen)
    #cv.imshow('mask',mask)

    mask_inv=cv.bitwise_not(mask)
    non_zero_points=cv.findNonZero(mask_inv)

    if non_zero_points is not None:
        for point in non_zero_points:
            clicker(monitor2['left']+point[0][0]+20,monitor2['top']+point[0][1]+20)
            click_counter += 1
            break


#loop
#time.sleep(5)
while True:
    lootpixelresult=lootof()

    if lootpixelresult:
        keyboard.press(Key.shift)

        while lootpixelresult:
            lootbox()
            lootpixelresult=lootbox()
    
        keyboard.release(Key.shift)

    print(f'loop: {loop_counter}, items clicked: {click_counter}', end='\r')
    loop_counter += 1

    if loop_counter == 9:
        loop_counter = 0

    time.sleep(.1)
    if cv.waitKey(1) & 0xFF == ord("q"):
        cv.destroyAllWindows()
        break
